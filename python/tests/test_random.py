"""Tests for the random module."""

import pytest

from demo.containers import Container
from demo.random import RandomGenerator, sample, shuffle


class TestRandomGeneratorBasic:
    """Test RandomGenerator creation and basic operations."""

    def test_default_constructor(self) -> None:
        """Test creating random generator with default constructor."""
        rg = RandomGenerator()

        value = rg.rand_int(1, 10)
        assert isinstance(value, int)
        assert 1 <= value <= 10

    def test_explicit_seed_constructor(self) -> None:
        """Test creating random generator with explicit seed."""
        seed = 12345

        rg1 = RandomGenerator(seed=seed)
        value1 = rg1.rand_int(1, 1000)

        rg2 = RandomGenerator(seed=seed)
        value2 = rg2.rand_int(1, 1000)

        assert value1 == value2


@pytest.fixture
def rg() -> RandomGenerator:
    """Fixture for random generator with fixed seed."""
    return RandomGenerator(seed=42)


class TestRandomGeneratorInt:
    """Test RandomGenerator integral number generation."""

    @pytest.mark.parametrize(
        'min_val, max_val',
        [
            (1, 100),
            (42, 42),
            (-100, -10),
            (1000000, 2000000),
        ],
    )
    def test_generate_int_values(
        self, rg: RandomGenerator, min_val: int, max_val: int
    ) -> None:
        """Test generating integer values in range."""
        for _ in range(100):
            value = rg.rand_int(min_val, max_val)

            assert isinstance(value, int)
            assert min_val <= value <= max_val


class TestRandomGeneratorFloat:
    """Test RandomGenerator floating-point number generation."""

    @pytest.mark.parametrize(
        'min_val, max_val',
        [
            (0.0, 1.0),
            (-10.0, -5.0),
            (0.0, 0.001),
        ],
    )
    def test_generate_float_values(
        self, rg: RandomGenerator, min_val: float, max_val: float
    ) -> None:
        """Test generating floating-point values in range."""
        for _ in range(100):
            value = rg.rand_float(min_val, max_val)
            assert isinstance(value, float)
            assert min_val <= value < max_val


class TestRandomGeneratorList:
    """Test RandomGenerator list generation."""

    @pytest.mark.parametrize(
        'min_val, max_val, count',
        [
            (1, 49, 10),
            (1, 10, 0),
        ],
    )
    def test_generate_int_list(
        self, rg: RandomGenerator, min_val: int, max_val: int, count: int
    ) -> None:
        """Test generating list of integers."""
        values = rg.rand_ints(min_val, max_val, count)

        assert isinstance(values, list)
        assert len(values) == count
        for value in values:
            assert isinstance(value, int)
            assert min_val <= value <= max_val

    @pytest.mark.parametrize(
        'min_val, max_val, count',
        [
            (0.0, 1.0, 10),
            (-10.0, -9.0, 0),
        ],
    )
    def test_generate_float_list(
        self, rg: RandomGenerator, min_val: float, max_val: float, count: int
    ) -> None:
        """Test generating list of floating-point values."""
        values = rg.rand_floats(min_val, max_val, count)

        assert isinstance(values, list)
        assert len(values) == count
        for value in values:
            assert isinstance(value, float)
            assert min_val <= value < max_val

    @pytest.mark.parametrize(
        'min_val, max_val, count',
        [
            (1, 100, 1000),
        ],
    )
    def test_generate_large_list(
        self, rg: RandomGenerator, min_val: int, max_val: int, count: int
    ) -> None:
        """Test generating large list."""
        values = rg.rand_ints(min_val, max_val, count)

        assert isinstance(values, list)
        assert len(values) == count

        # Check that we get some distribution (not all the same value)
        first_value = values[0]
        has_different = any(value != first_value for value in values)
        assert has_different  # Should have some variety

    def test_generate_negative_count(self, rg: RandomGenerator) -> None:
        """Test generating list with negative count."""
        with pytest.raises(ValueError, match='Count must be non-negative'):
            rg.rand_ints(1, 10, -1)

        with pytest.raises(ValueError, match='Count must be non-negative'):
            rg.rand_floats(0.0, 1.0, -1)


class TestRandomGeneratorBool:
    """Test RandomGenerator boolean generation."""

    @pytest.mark.parametrize(
        'probability',
        [
            0.5,
            0.9,
            0.1,
        ],
    )
    def test_coin_flips(self, rg: RandomGenerator, probability: float) -> None:
        """Test fair coin flip (50% probability)."""
        iterations = 1000

        true_count = 0
        for _ in range(iterations):
            if rg.rand_bool(probability):
                true_count += 1

        # Should be roughly the probability (allow some variance)
        assert true_count > int(iterations * (probability - 0.2))
        assert true_count < int(iterations * (probability + 0.2))

    def test_extreme_probabilities(self, rg: RandomGenerator) -> None:
        """Test extreme probability values."""
        iterations = 1000

        # Test 0% probability
        for _ in range(iterations):
            assert rg.rand_bool(0.0) is False

        # Test 100% probability
        for _ in range(iterations):
            assert rg.rand_bool(1.0) is True

    def test_invalid_probability(self, rg: RandomGenerator) -> None:
        """Test invalid probability values."""
        with pytest.raises(ValueError, match='Probability must be between 0.0 and 1.0'):
            rg.rand_bool(-0.5)

        with pytest.raises(ValueError, match='Probability must be between 0.0 and 1.0'):
            rg.rand_bool(10.0)


class TestRandomGeneratorNormal:
    """Test RandomGenerator normal distribution generation."""

    def test_standard_normal_distribution(self, rg: RandomGenerator) -> None:
        """Test standard normal distribution."""
        mean = 0.0
        stddev = 1.0
        iterations = 1000

        values = []
        for _ in range(iterations):
            values.append(rg.normal(mean, stddev))

        # Calculate sample mean and verify it's close to expected
        sample_mean = sum(values) / len(values)

        # Sample mean should be close to theoretical mean (within 0.2)
        assert abs(sample_mean - mean) < 0.2

        # Most values should be within 3 standard deviations
        within_3_sigma = sum(1 for val in values if abs(val - mean) <= 3.0 * stddev)
        # At least 99% should be within 3 sigma
        assert within_3_sigma > int(iterations * 0.99)

    @pytest.mark.parametrize(
        'mean, stddev',
        [
            (100.0, 15.0),
            (5.0, 0.1),
        ],
    )
    def test_custom_normal_distribution(
        self, rg: RandomGenerator, mean: float, stddev: float
    ) -> None:
        """Test custom normal distribution parameters."""
        value = rg.normal(mean, stddev)

        # Value should be reasonable (within 5 standard deviations)
        assert value > mean - (5.0 * stddev)
        assert value < mean + (5.0 * stddev)


class TestRandomGeneratorSeeding:
    """Test RandomGenerator seeding functionality."""

    def test_manual_seeding_deterministic(self) -> None:
        """Test that manual seeding produces deterministic results."""
        seed = 12345

        rg1 = RandomGenerator(seed=seed)
        rg2 = RandomGenerator(seed=seed)

        # Generate several values from both generators
        for _ in range(10):
            val1 = rg1.rand_int(1, 1000)
            val2 = rg2.rand_int(1, 1000)

            assert val1 == val2

    def test_reseeding_resets_sequence(self) -> None:
        """Test that re-seeding resets the sequence."""
        seed = 54321

        rg = RandomGenerator()
        rg.seed(seed)
        first_sequence = rg.rand_ints(1, 100, 100)

        rg.seed(seed)  # Reset with same seed
        second_sequence = rg.rand_ints(1, 100, 100)

        assert first_sequence == second_sequence

    def test_seed_with_time_non_deterministic(self) -> None:
        """Test that seed_with_time provides non-deterministic behavior."""
        import time

        rg = RandomGenerator()
        rg.seed_with_time()
        first_sequence = rg.rand_ints(1, 100, 100)

        time.sleep(0.01)
        rg.seed_with_time()
        second_sequence = rg.rand_ints(1, 100, 100)

        assert first_sequence != second_sequence


class TestShuffleContainer:
    """Test shuffle container functionality."""

    def test_shuffle_list(self) -> None:
        """Test shuffling a list."""
        original = [x for x in range(1, 21)]

        shuffled = original.copy()
        shuffle(shuffled)

        # Size should remain the same
        assert len(shuffled) == len(original)

        # Should be different (very high probability)
        assert shuffled != original

        # Should contain all the same elements
        assert sorted(shuffled) == sorted(original)

    def test_shuffle_container(self) -> None:
        """Test shuffling container."""
        data = [x for x in range(1, 21)]
        container = Container(int, data)

        shuffled = Container(int, data.copy())
        shuffle(shuffled)

        # Size should remain the same
        assert len(shuffled) == len(container)

        # Should be different (very high probability)
        assert shuffled != container

        # Should contain all the same elements
        assert sorted(shuffled) == list(container)

    def test_shuffle_single_element(self) -> None:
        """Test shuffling single element."""
        single = Container(int, [42])
        shuffle(single)

        assert len(single) == 1
        assert list(single) == [42]

    def test_shuffle_empty_container(self) -> None:
        """Test shuffling empty container."""
        empty = Container(int, [])
        shuffle(empty)

        assert list(empty) == []


class TestSampleFromRange:
    """Test sample from range functionality."""

    def test_sample_from_list(self) -> None:
        """Test sampling from a list."""
        population = [x for x in range(1, 11)]
        sample_size = 3

        sample_result = sample(population, sample_size)

        assert isinstance(sample_result, list)
        assert len(sample_result) == sample_size

        # All sampled elements should be from the original population
        for element in sample_result:
            assert element in population

    def test_sample_from_container(self) -> None:
        """Test sampling from a container."""
        container = Container(float, [float(x) for x in range(1, 11)])
        sample_size = 5

        sample_result = sample(container, sample_size)

        assert isinstance(sample_result, list)
        assert len(sample_result) == sample_size

        # All sampled elements should be from the original population
        for element in sample_result:
            assert element in container

    def test_sample_more_than_available(self) -> None:
        """Test sampling more than available elements."""
        small_population = Container(int, [x for x in range(1, 4)])
        large_sample_size = 10

        sample_result = sample(small_population, large_sample_size)

        # Should return all available elements
        assert sorted(sample_result) == list(small_population)

    def test_sample_zero_elements(self) -> None:
        """Test sampling zero elements."""
        population = Container(int, [x for x in range(1, 6)])

        sample_result = sample(population, 0)
        assert sample_result == []

    def test_sample_from_empty_range(self) -> None:
        """Test sampling from empty range."""
        empty_population = Container(int, [])

        sample_result = sample(empty_population, 5)
        assert sample_result == []
