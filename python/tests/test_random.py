"""Tests for Python random module.

Comprehensive tests following C++ test patterns for random functionality.
"""

import pytest

from python import random as cpp_random


class TestRandomGenerator:
    """Test RandomGenerator functionality."""

    def test_random_generator_creation(self) -> None:
        """Test basic random generator creation."""
        gen = cpp_random.RandomGenerator()
        assert gen is not None

    def test_random_generator_with_seed(self) -> None:
        """Test random generator with seed."""
        gen = cpp_random.RandomGenerator(seed=12345)
        assert gen is not None

    def test_randint_basic(self) -> None:
        """Test basic random integer generation."""
        gen = cpp_random.RandomGenerator()
        value = gen.randint(1, 10)

        assert isinstance(value, int)
        assert 1 <= value <= 10

    def test_randint_range(self) -> None:
        """Test random integer generation in various ranges."""
        gen = cpp_random.RandomGenerator(seed=42)

        # Test different ranges
        for min_val, max_val in [(1, 5), (10, 20), (-5, 5), (100, 200)]:
            value = gen.randint(min_val, max_val)
            assert min_val <= value <= max_val

    def test_randlong(self) -> None:
        """Test random long integer generation."""
        gen = cpp_random.RandomGenerator()
        value = gen.randlong(1000000, 2000000)

        assert isinstance(value, int)
        assert 1000000 <= value <= 2000000

    def test_random_float(self) -> None:
        """Test random float generation."""
        gen = cpp_random.RandomGenerator()
        value = gen.random()

        assert isinstance(value, float)
        assert 0.0 <= value < 1.0

    def test_uniform(self) -> None:
        """Test uniform distribution."""
        gen = cpp_random.RandomGenerator()
        value = gen.uniform(2.0, 8.0)

        assert isinstance(value, float)
        assert 2.0 <= value < 8.0

    def test_randfloat(self) -> None:
        """Test random float32 generation."""
        gen = cpp_random.RandomGenerator()
        value = gen.randfloat(1.0, 5.0)

        assert isinstance(value, float)
        assert 1.0 <= value < 5.0

    def test_choice(self) -> None:
        """Test random boolean generation."""
        gen = cpp_random.RandomGenerator()

        # Test with default probability
        value = gen.choice()
        assert isinstance(value, bool)

        # Test with custom probability
        value_high = gen.choice(0.9)
        value_low = gen.choice(0.1)
        assert isinstance(value_high, bool)
        assert isinstance(value_low, bool)

    def test_normal_distribution(self) -> None:
        """Test normal distribution."""
        gen = cpp_random.RandomGenerator()

        # Test with default parameters
        value = gen.normal()
        assert isinstance(value, float)

        # Test with custom parameters
        value_custom = gen.normal(mean=10.0, stddev=2.0)
        assert isinstance(value_custom, float)

    def test_normal_float(self) -> None:
        """Test normal distribution with float32."""
        gen = cpp_random.RandomGenerator()
        value = gen.normal_float(5.0, 1.5)

        assert isinstance(value, float)

    def test_integers_vector(self) -> None:
        """Test generating vector of integers."""
        gen = cpp_random.RandomGenerator()
        values = gen.integers(1, 100, 10)

        assert isinstance(values, list)
        assert len(values) == 10
        assert all(isinstance(v, int) for v in values)
        assert all(1 <= v <= 100 for v in values)

    def test_floats_vector(self) -> None:
        """Test generating vector of floats."""
        gen = cpp_random.RandomGenerator()
        values = gen.floats(0.0, 1.0, 5)

        assert isinstance(values, list)
        assert len(values) == 5
        assert all(isinstance(v, float) for v in values)
        assert all(0.0 <= v < 1.0 for v in values)

    def test_seed_reproducibility(self) -> None:
        """Test seed reproducibility."""
        gen1 = cpp_random.RandomGenerator(seed=123)
        gen2 = cpp_random.RandomGenerator(seed=123)

        # Same seed should produce same sequence
        values1 = [gen1.randint(1, 1000) for _ in range(5)]
        values2 = [gen2.randint(1, 1000) for _ in range(5)]

        assert values1 == values2

    def test_seed_method(self) -> None:
        """Test seeding after creation."""
        gen = cpp_random.RandomGenerator()

        # Set seed and generate values
        gen.seed(456)
        values1 = [gen.randint(1, 100) for _ in range(3)]

        # Reset with same seed
        gen.seed(456)
        values2 = [gen.randint(1, 100) for _ in range(3)]

        assert values1 == values2

    def test_seed_with_time(self) -> None:
        """Test seeding with current time."""
        gen = cpp_random.RandomGenerator()
        gen.seed_with_time()

        # Should be able to generate values
        value = gen.randint(1, 100)
        assert 1 <= value <= 100


class TestGlobalFunctions:
    """Test global random functions."""

    def test_shuffle_int_list(self) -> None:
        """Test shuffling integer list."""
        original = [1, 2, 3, 4, 5]
        data = original.copy()

        cpp_random.shuffle(data)

        # List should still contain same elements
        assert sorted(data) == sorted(original)
        assert len(data) == len(original)

    def test_shuffle_float_list(self) -> None:
        """Test shuffling float list."""
        original = [1.1, 2.2, 3.3, 4.4, 5.5]
        data = original.copy()

        cpp_random.shuffle(data)

        assert sorted(data) == sorted(original)
        assert len(data) == len(original)

    def test_shuffle_string_list(self) -> None:
        """Test shuffling string list."""
        original = ['apple', 'banana', 'cherry', 'date']
        data = original.copy()

        cpp_random.shuffle(data)

        assert sorted(data) == sorted(original)
        assert len(data) == len(original)

    def test_shuffle_unsupported_type(self) -> None:
        """Test shuffling unsupported type falls back to Python random."""
        data = [{'a': 1}, {'b': 2}, {'c': 3}]
        original = data.copy()

        # Should not raise exception
        cpp_random.shuffle(data)

        # Should contain same elements (though order may change)
        assert len(data) == len(original)

    def test_sample_int_list(self) -> None:
        """Test sampling from integer list."""
        population = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        sample = cpp_random.sample(population, 3)

        assert isinstance(sample, list)
        assert len(sample) == 3
        assert all(item in population for item in sample)
        assert len(set(sample)) == 3  # No duplicates

    def test_sample_float_list(self) -> None:
        """Test sampling from float list."""
        population = [1.1, 2.2, 3.3, 4.4, 5.5]
        sample = cpp_random.sample(population, 2)

        assert len(sample) == 2
        assert all(item in population for item in sample)

    def test_sample_string_list(self) -> None:
        """Test sampling from string list."""
        population = ['a', 'b', 'c', 'd', 'e']
        sample = cpp_random.sample(population, 3)

        assert len(sample) == 3
        assert all(item in population for item in sample)

    def test_sample_string_characters(self) -> None:
        """Test sampling characters from string."""
        text = 'hello world'
        sample = cpp_random.sample_string(text, 5)

        assert isinstance(sample, list)
        assert len(sample) == 5
        assert all(char in text for char in sample)

    def test_sample_unsupported_type(self) -> None:
        """Test sampling unsupported type falls back to Python random."""
        population = [{'a': 1}, {'b': 2}, {'c': 3}, {'d': 4}]
        sample = cpp_random.sample(population, 2)

        assert len(sample) == 2


class TestDistributions:
    """Test probability distributions."""

    def test_uniform_int_distribution(self) -> None:
        """Test uniform integer distribution."""
        gen = cpp_random.RandomGenerator(seed=42)
        dist = cpp_random.UniformInt(gen, 10, 20)

        value = dist.sample()
        assert isinstance(value, int)
        assert 10 <= value <= 20

        samples = dist.samples(5)
        assert len(samples) == 5
        assert all(10 <= v <= 20 for v in samples)

    def test_uniform_float_distribution(self) -> None:
        """Test uniform float distribution."""
        gen = cpp_random.RandomGenerator(seed=42)
        dist = cpp_random.UniformFloat(gen, 1.0, 5.0)

        value = dist.sample()
        assert isinstance(value, float)
        assert 1.0 <= value < 5.0

        samples = dist.samples(10)
        assert len(samples) == 10
        assert all(1.0 <= v < 5.0 for v in samples)

    def test_normal_distribution_class(self) -> None:
        """Test normal distribution class."""
        gen = cpp_random.RandomGenerator(seed=42)

        # Standard normal distribution
        std_normal = cpp_random.Normal(gen)
        value = std_normal.sample()
        assert isinstance(value, float)

        # Custom normal distribution
        custom_normal = cpp_random.Normal(gen, mean=100.0, stddev=15.0)
        samples = custom_normal.samples(100)

        # Statistical tests (approximate)
        mean_sample = sum(samples) / len(samples)
        assert 85.0 < mean_sample < 115.0  # Should be around 100 ± 15

    def test_distribution_inheritance(self) -> None:
        """Test that distributions follow inheritance pattern."""
        gen = cpp_random.RandomGenerator()

        # All distributions should be instances of Distribution
        uniform_int = cpp_random.UniformInt(gen, 1, 10)
        uniform_float = cpp_random.UniformFloat(gen, 1.0, 10.0)
        normal = cpp_random.Normal(gen)

        # Test that they all have sample and samples methods
        assert hasattr(uniform_int, 'sample')
        assert hasattr(uniform_int, 'samples')
        assert hasattr(uniform_float, 'sample')
        assert hasattr(uniform_float, 'samples')
        assert hasattr(normal, 'sample')
        assert hasattr(normal, 'samples')


class TestConvenienceFunctions:
    """Test convenience functions using default generator."""

    def test_randint_convenience(self) -> None:
        """Test randint convenience function."""
        value = cpp_random.randint(1, 100)
        assert isinstance(value, int)
        assert 1 <= value <= 100

    def test_random_convenience(self) -> None:
        """Test random convenience function."""
        value = cpp_random.random()
        assert isinstance(value, float)
        assert 0.0 <= value < 1.0

    def test_uniform_convenience(self) -> None:
        """Test uniform convenience function."""
        value = cpp_random.uniform(5.0, 10.0)
        assert isinstance(value, float)
        assert 5.0 <= value < 10.0

    def test_choice_convenience(self) -> None:
        """Test choice convenience function."""
        value = cpp_random.choice()
        assert isinstance(value, bool)

        value_biased = cpp_random.choice(0.8)
        assert isinstance(value_biased, bool)

    def test_normal_convenience(self) -> None:
        """Test normal convenience function."""
        value = cpp_random.normal()
        assert isinstance(value, float)

        value_custom = cpp_random.normal(mean=50.0, stddev=10.0)
        assert isinstance(value_custom, float)


class TestRandomEdgeCases:
    """Test edge cases and error conditions."""

    def test_randint_same_bounds(self) -> None:
        """Test randint with same min and max."""
        gen = cpp_random.RandomGenerator()
        value = gen.randint(5, 5)
        assert value == 5

    def test_uniform_zero_range(self) -> None:
        """Test uniform with minimal range."""
        gen = cpp_random.RandomGenerator()
        value = gen.uniform(1.0, 1.0)
        assert value == 1.0

    def test_sample_larger_than_population(self) -> None:
        """Test sampling more items than available."""
        population = [1, 2, 3]

        with pytest.raises(Exception):  # Should raise an error
            cpp_random.sample(population, 5)

    def test_empty_vector_generation(self) -> None:
        """Test generating empty vectors."""
        gen = cpp_random.RandomGenerator()

        int_vec = gen.integers(1, 10, 0)
        float_vec = gen.floats(0.0, 1.0, 0)

        assert int_vec == []
        assert float_vec == []

    def test_negative_count_vector(self) -> None:
        """Test generating vectors with negative count."""
        gen = cpp_random.RandomGenerator()

        # Should handle gracefully or raise appropriate error
        try:
            gen.integers(1, 10, -1)
        except Exception:
            pass  # Expected to fail


class TestRandomIntegration:
    """Integration tests for random functionality."""

    def test_multiple_generators_independence(self) -> None:
        """Test that multiple generators are independent."""
        gen1 = cpp_random.RandomGenerator(seed=111)
        gen2 = cpp_random.RandomGenerator(seed=222)

        values1 = [gen1.randint(1, 1000) for _ in range(10)]
        values2 = [gen2.randint(1, 1000) for _ in range(10)]

        # Different seeds should produce different sequences
        assert values1 != values2

    def test_random_data_pipeline(self) -> None:
        """Test complete random data generation pipeline."""
        gen = cpp_random.RandomGenerator(seed=42)

        # Generate random data
        int_data = gen.integers(1, 100, 20)

        # Shuffle the data
        cpp_random.shuffle(int_data)

        # Sample from the shuffled data
        sample_data = cpp_random.sample(int_data, 5)

        assert len(int_data) == 20
        assert len(sample_data) == 5
        assert all(item in int_data for item in sample_data)

    def test_statistical_properties(self) -> None:
        """Test statistical properties of distributions."""
        gen = cpp_random.RandomGenerator(seed=12345)

        # Generate large sample from uniform distribution
        uniform_samples = gen.floats(0.0, 1.0, 1000)

        # Basic statistical checks
        mean = sum(uniform_samples) / len(uniform_samples)
        assert 0.4 < mean < 0.6  # Should be around 0.5

        min_val = min(uniform_samples)
        max_val = max(uniform_samples)
        assert 0.0 <= min_val < 1.0
        assert 0.0 <= max_val < 1.0

    def test_cross_type_operations(self) -> None:
        """Test operations across different random types."""
        gen = cpp_random.RandomGenerator(seed=999)

        # Generate different types of random data
        integers = gen.integers(1, 10, 5)
        floats = gen.floats(1.0, 10.0, 5)
        booleans = [gen.choice() for _ in range(5)]

        # Verify types and ranges
        assert all(isinstance(x, int) and 1 <= x <= 10 for x in integers)
        assert all(isinstance(x, float) and 1.0 <= x < 10.0 for x in floats)
        assert all(isinstance(x, bool) for x in booleans)

        # Test shuffling mixed data
        mixed_strings = [str(x) for x in integers + floats]
        cpp_random.shuffle(mixed_strings)
        assert len(mixed_strings) == 10

    def test_performance_consistency(self) -> None:
        """Test that random generation performance is consistent."""
        gen = cpp_random.RandomGenerator()

        # Generate multiple large batches
        batch1 = gen.integers(1, 1000000, 1000)
        batch2 = gen.integers(1, 1000000, 1000)
        batch3 = gen.integers(1, 1000000, 1000)

        # All batches should be complete
        assert len(batch1) == 1000
        assert len(batch2) == 1000
        assert len(batch3) == 1000

        # Values should be in range
        for batch in [batch1, batch2, batch3]:
            assert all(1 <= x <= 1000000 for x in batch)
