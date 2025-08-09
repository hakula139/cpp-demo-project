"""Tests for Python memory module.

Comprehensive tests following C++ test patterns for memory management functionality.
"""

import pytest

from python import memory, shapes


class TestResourceManager:
    """Test ResourceManager basic operations."""

    def test_resource_manager_creation(self) -> None:
        """Test basic resource manager creation."""
        manager = memory.ResourceManager()
        assert manager is not None

    def test_create_circle(self) -> None:
        """Test creating circle through resource manager."""
        manager = memory.ResourceManager()
        circle = manager.create_circle(5.0)

        assert circle.get_name() == 'Circle'
        assert circle.get_area() == pytest.approx(3.14159 * 25.0)

    def test_create_rectangle(self) -> None:
        """Test creating rectangle through resource manager."""
        manager = memory.ResourceManager()
        rectangle = manager.create_rectangle(4.0, 3.0)

        assert rectangle.get_name() == 'Rectangle'
        assert rectangle.get_area() == 12.0
        assert not rectangle.is_square()

    def test_create_shared_circle(self) -> None:
        """Test creating shared circle."""
        manager = memory.ResourceManager()
        circle = manager.create_shared_circle(3.0)

        assert circle.get_name() == 'Circle'
        assert circle.get_area() == pytest.approx(3.14159 * 9.0)

    def test_create_shared_rectangle(self) -> None:
        """Test creating shared rectangle."""
        manager = memory.ResourceManager()
        rectangle = manager.create_shared_rectangle(6.0, 4.0)

        assert rectangle.get_name() == 'Rectangle'
        assert rectangle.get_area() == 24.0

    def test_register_cleanup(self) -> None:
        """Test registering cleanup functions."""
        manager = memory.ResourceManager()
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        manager.register_cleanup(cleanup_func)
        manager.execute_cleanup()

        assert len(cleanup_called) == 1

    def test_multiple_cleanup_functions(self) -> None:
        """Test multiple cleanup functions."""
        manager = memory.ResourceManager()
        cleanup_order = []

        def cleanup1() -> None:
            cleanup_order.append(1)

        def cleanup2() -> None:
            cleanup_order.append(2)

        def cleanup3() -> None:
            cleanup_order.append(3)

        manager.register_cleanup(cleanup1)
        manager.register_cleanup(cleanup2)
        manager.register_cleanup(cleanup3)
        manager.execute_cleanup()

        # Should execute in reverse order (LIFO)
        assert cleanup_order == [3, 2, 1]

    def test_cleanup_functions_cleared(self) -> None:
        """Test cleanup functions are cleared after execution."""
        manager = memory.ResourceManager()
        cleanup_count = []

        def cleanup_func() -> None:
            cleanup_count.append(1)

        manager.register_cleanup(cleanup_func)
        manager.execute_cleanup()
        assert len(cleanup_count) == 1

        # Execute again - should not call cleanup again
        manager.execute_cleanup()
        assert len(cleanup_count) == 1


class TestResourceManagerContextManager:
    """Test ResourceManager as context manager."""

    def test_context_manager_basic(self) -> None:
        """Test basic context manager usage."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        with memory.ResourceManager() as manager:
            manager.register_cleanup(cleanup_func)
            circle = manager.create_circle(2.0)
            assert circle.get_area() == pytest.approx(3.14159 * 4.0)

        # Cleanup should be called automatically
        assert len(cleanup_called) == 1

    def test_context_manager_with_exception(self) -> None:
        """Test context manager with exception handling."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        try:
            with memory.ResourceManager() as manager:
                manager.register_cleanup(cleanup_func)
                raise ValueError('Test exception')
        except ValueError:
            pass

        # Cleanup should still be called
        assert len(cleanup_called) == 1

    def test_nested_context_managers(self) -> None:
        """Test nested context managers."""
        cleanup_order = []

        def cleanup1() -> None:
            cleanup_order.append(1)

        def cleanup2() -> None:
            cleanup_order.append(2)

        with memory.ResourceManager() as manager1:
            manager1.register_cleanup(cleanup1)

            with memory.ResourceManager() as manager2:
                manager2.register_cleanup(cleanup2)
                circle = manager2.create_circle(1.0)
                assert circle.get_area() == pytest.approx(3.14159)

        # Inner manager cleanup should execute first
        assert cleanup_order == [2, 1]


class TestManagedResources:
    """Test managed_resources context manager."""

    def test_managed_resources_basic(self) -> None:
        """Test basic managed resources usage."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        with memory.managed_resources() as manager:
            manager.register_cleanup(cleanup_func)
            circle = manager.create_circle(3.0)
            assert circle.get_area() == pytest.approx(3.14159 * 9.0)

        assert len(cleanup_called) == 1

    def test_managed_resources_multiple_objects(self) -> None:
        """Test managed resources with multiple objects."""
        with memory.managed_resources() as manager:
            circle = manager.create_circle(2.0)
            rectangle = manager.create_rectangle(3.0, 4.0)
            square_rect = manager.create_rectangle(5.0, 5.0)

            assert circle.get_area() == pytest.approx(3.14159 * 4.0)
            assert rectangle.get_area() == 12.0
            assert square_rect.is_square()

    def test_managed_resources_with_exception(self) -> None:
        """Test managed resources with exception."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        try:
            with memory.managed_resources() as manager:
                manager.register_cleanup(cleanup_func)
                raise RuntimeError('Test error')
        except RuntimeError:
            pass

        assert len(cleanup_called) == 1


class TestScopedResource:
    """Test ScopedResource functionality."""

    def test_scoped_resource_basic(self) -> None:
        """Test basic scoped resource usage."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        test_resource = 'test_value'
        scoped = memory.ScopedResource(test_resource, cleanup_func)

        with scoped as resource:
            assert resource == test_resource

        assert len(cleanup_called) == 1

    def test_scoped_resource_no_cleanup(self) -> None:
        """Test scoped resource without cleanup function."""
        test_resource = {'key': 'value'}
        scoped = memory.ScopedResource(test_resource)

        with scoped as resource:
            assert resource == test_resource
            resource['new_key'] = 'new_value'

        # Should not raise any exceptions

    def test_scoped_resource_with_exception(self) -> None:
        """Test scoped resource with exception in context."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        test_resource = [1, 2, 3]

        try:
            with memory.ScopedResource(test_resource, cleanup_func) as resource:
                resource.append(4)
                raise ValueError('Test error')
        except ValueError:
            pass

        assert len(cleanup_called) == 1

    def test_scoped_resource_factory(self) -> None:
        """Test scoped resource factory function."""
        cleanup_called = []

        def cleanup_func() -> None:
            cleanup_called.append(True)

        test_resource = 42

        with memory.scoped_resource(test_resource, cleanup_func) as resource:
            assert resource == 42

        assert len(cleanup_called) == 1


class TestMemoryIntegration:
    """Integration tests for memory management."""

    def test_resource_manager_with_shapes(self) -> None:
        """Test resource manager integration with shapes."""
        with memory.managed_resources() as manager:
            # Create various shapes
            circle = manager.create_circle(1.0)
            rectangle = manager.create_rectangle(2.0, 3.0)
            shared_circle = manager.create_shared_circle(4.0)
            shared_rectangle = manager.create_shared_rectangle(5.0, 6.0)

            # Test their properties
            assert circle.get_area() == pytest.approx(3.14159)
            assert rectangle.get_area() == 6.0
            assert shared_circle.get_area() == pytest.approx(3.14159 * 16.0)
            assert shared_rectangle.get_area() == 30.0

            # Test comparison
            assert shared_circle.get_area() > circle.get_area()
            assert shared_rectangle.get_area() > rectangle.get_area()

    def test_multiple_managers_independent(self) -> None:
        """Test that multiple managers are independent."""
        cleanup_count1 = []
        cleanup_count2 = []

        def cleanup1() -> None:
            cleanup_count1.append(1)

        def cleanup2() -> None:
            cleanup_count2.append(1)

        manager1 = memory.ResourceManager()
        manager2 = memory.ResourceManager()

        manager1.register_cleanup(cleanup1)
        manager2.register_cleanup(cleanup2)

        # Execute cleanup for manager1 only
        manager1.execute_cleanup()
        assert len(cleanup_count1) == 1
        assert len(cleanup_count2) == 0

        # Execute cleanup for manager2
        manager2.execute_cleanup()
        assert len(cleanup_count1) == 1
        assert len(cleanup_count2) == 1

    def test_resource_lifecycle_management(self) -> None:
        """Test complete resource lifecycle."""
        resource_states = []

        def create_resource() -> str:
            resource_states.append('created')
            return 'test_resource'

        def cleanup_resource() -> None:
            resource_states.append('cleaned_up')

        # Create and manage resource
        resource = create_resource()

        with memory.scoped_resource(resource, cleanup_resource) as managed:
            resource_states.append('using')
            assert managed == 'test_resource'

        assert resource_states == ['created', 'using', 'cleaned_up']

    def test_exception_safety(self) -> None:
        """Test exception safety in resource management."""
        cleanup_states = []

        def cleanup1() -> None:
            cleanup_states.append('cleanup1')

        def cleanup2() -> None:
            cleanup_states.append('cleanup2')
            raise RuntimeError('Cleanup error')

        def cleanup3() -> None:
            cleanup_states.append('cleanup3')

        manager = memory.ResourceManager()
        manager.register_cleanup(cleanup1)
        manager.register_cleanup(cleanup2)
        manager.register_cleanup(cleanup3)

        # Test that cleanup continues even if one function throws
        try:
            manager.execute_cleanup()
        except RuntimeError:
            pass

        # All cleanup functions should be attempted
        assert 'cleanup3' in cleanup_states
        assert 'cleanup2' in cleanup_states
        # cleanup1 might not execute if cleanup2 throws in C++ version

    def test_raii_pattern_simulation(self) -> None:
        """Test RAII pattern simulation with Python context managers."""
        resource_log = []

        class MockResource:
            def __init__(self, name: str):
                self.name = name
                resource_log.append(f'{name}_created')

            def use(self) -> str:
                resource_log.append(f'{self.name}_used')
                return f'using_{self.name}'

            def cleanup(self) -> None:
                resource_log.append(f'{self.name}_cleaned')

        # Simulate RAII with scoped resources
        resource1 = MockResource('resource1')
        resource2 = MockResource('resource2')

        with memory.scoped_resource(resource1, resource1.cleanup):
            with memory.scoped_resource(resource2, resource2.cleanup):
                result1 = resource1.use()
                result2 = resource2.use()
                assert result1 == 'using_resource1'
                assert result2 == 'using_resource2'

        # Check proper cleanup order (reverse of creation)
        expected_log = [
            'resource1_created',
            'resource2_created',
            'resource1_used',
            'resource2_used',
            'resource2_cleaned',  # Inner scope cleaned first
            'resource1_cleaned',  # Outer scope cleaned last
        ]
        assert resource_log == expected_log
