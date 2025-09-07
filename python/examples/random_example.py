"""Example demonstrating the usage of the random module."""

from demo.containers import Container
from demo.random import RandomGenerator, sample, shuffle


def demonstrate_basic_generation() -> None:
    """Demonstrate basic random number generation."""
    print('\n1. Basic Random Generation:')
    generator = RandomGenerator(seed=42)  # Fixed seed for reproducible output

    # Integer generation
    dice_roll = generator.rand_int(1, 6)
    lottery_number = generator.rand_int(1, 49)
    print(f'   Dice roll: {dice_roll}, Lottery number: {lottery_number}')

    # Float generation
    probability = generator.rand_float(0.0, 1.0)
    temperature = generator.rand_float(-10.0, 40.0)
    print(f'   Probability: {probability:.3f}, Temperature: {temperature:.1f}°C')

    # Boolean generation
    coin_flip = 'Heads' if generator.rand_bool() else 'Tails'
    rare_event = 'Occurred' if generator.rand_bool(0.1) else "Didn't occur"
    print(f'   Coin flip: {coin_flip}, Rare event: {rare_event}')


def demonstrate_probability_distributions() -> None:
    """Demonstrate various probability distributions."""
    print('\n2. Probability Distributions:')
    generator = RandomGenerator(seed=42)

    # Uniform distributions
    uniform_ints = generator.rand_ints(10, 20, count=5)
    print(f'   Uniform integers [10, 20]: {uniform_ints}')

    uniform_reals = generator.rand_floats(0.5, 1.5, count=3)
    print(f'   Uniform reals [0.5, 1.5): {uniform_reals}')

    # Normal distribution
    iq_scores: list[int] = []
    for _ in range(5):
        iq_scores.append(int(generator.normal(100.0, 15.0)))
    print(f'   IQ scores (μ = 100, σ = 15): {iq_scores}')

    # Bernoulli distribution
    coin_flips: list[str] = []
    for _ in range(10):
        coin_flips.append('H' if generator.rand_bool(0.7) else 'T')
    print(f'   Biased coin flips (p = 0.7): {coin_flips}')


def demonstrate_seeding_and_reproducibility() -> None:
    """Demonstrate seeding and reproducibility."""
    print('\n3. Seeding and Reproducibility:')
    rg1 = RandomGenerator(seed=123)
    value1 = rg1.rand_int(1, 1000)
    rg2 = RandomGenerator(seed=123)
    value2 = rg2.rand_int(1, 1000)
    print(f'   Same seed (123): {value1} == {value2}')

    rg3 = RandomGenerator()  # Random seed
    rg3.seed_with_time()
    time_seeded = rg3.rand_int(1, 1000)
    print(f'   Time-seeded value: {time_seeded}')


def demonstrate_range_operations() -> None:
    """Demonstrate range operations (shuffling and sampling)."""
    print('\n4. Range Operations:')

    # Container shuffling
    deck = Container(int, [x for x in range(1, 9)])
    print(f'   Original deck: {deck}')

    shuffled_deck = Container(int, [x for x in range(1, 9)])
    shuffle(shuffled_deck)
    print(f'   Shuffled deck: {shuffled_deck}')

    # Random sampling
    population = Container(int, [x for x in range(10, 100, 10)])
    sample_result = Container(int, sample(population, 4))
    print(f'   Sample from population: {sample_result}')

    # String sampling
    vowels = Container(str, ['A', 'E', 'I', 'O', 'U'])
    vowel_sample = Container(str, sample(vowels, 3))
    print(f'   Random vowels: {vowel_sample}')


def demonstrate_practical_applications() -> None:
    """Demonstrate practical applications of random generation."""
    print('\n5. Practical Applications:')
    dice_generator = RandomGenerator()

    # Simulate rolling multiple dice
    dice_rolls = Container(int, dice_generator.rand_ints(1, 6, count=3))
    total = sum(dice_rolls)
    print(f'   Rolling 3 dice: {dice_rolls}, Total: {total}')

    # Monte Carlo estimation (simple π approximation)
    inside_circle = 0
    samples = 10000
    for _ in range(samples):
        x = dice_generator.rand_float(-1.0, 1.0)
        y = dice_generator.rand_float(-1.0, 1.0)
        if x * x + y * y <= 1.0:
            inside_circle += 1

    pi_estimate = 4.0 * inside_circle / samples
    print(f'   Monte Carlo π estimation ({samples} samples): {pi_estimate:.3f}')


def main() -> None:
    """Run all random examples."""
    print('=== Random Module Example ===')

    demonstrate_basic_generation()
    demonstrate_probability_distributions()
    demonstrate_seeding_and_reproducibility()
    demonstrate_range_operations()
    demonstrate_practical_applications()

    print('\n=== Random Module Example Completed ===')


if __name__ == '__main__':
    main()
