import matplotlib.pyplot as plt
import numpy as np

# Data for provinces (sorted by overall obesity rate for consistency)
provinces = [
    "British Columbia", "Quebec", "Ontario", "Alberta", "Manitoba",
    "Nova Scotia", "Saskatchewan", "New Brunswick", "Newfoundland and Labrador"
]

# Obesity rates
overall_obesity_rates = [23.2, 26.2, 28.3, 29.7, 32.7, 34.8, 35.6, 37.3, 40.4]
male_obesity_rates = [24.4, 26.9, 28.4, 31.5, 32.0, 35.2, 37.0, 34.4, 41.2]
female_obesity_rates = [22.0, 25.6, 28.1, 27.8, 33.5, 34.5, 34.0, 40.2, 39.5]

# Life expectancies (2023)
overall_life_expectancies = [81.95, 82.55, 82.33, 80.66, 79.10, 80.48, 78.99, 80.44, 79.11]
male_life_expectancies = [79.31, 80.77, 80.13, 78.34, 76.90, 78.33, 76.70, 78.42, 77.08]
female_life_expectancies = [84.71, 84.30, 84.52, 83.10, 81.46, 82.67, 81.40, 82.50, 81.13]

# Past 12-month alcohol use (sorted by overall obesity rate order)
alcohol_use = [78.5, 84.2, 75.6, 78.8, 71.0, 73.8, 78.4, 74.9, 77.4]

# URLs for the bottom of the plots
urls = [
    "https://www.canada.ca/en/public-health/services/publications/healthy-living/obesity-statistics-canada.html",
    "https://en.wikipedia.org/wiki/List_of_Canadian_provinces_and_territories_by_life_expectancy",
    "https://health-infobase.canada.ca/alcohol/ctads/"
]

def create_scatter_plot(x_data, y_data, provinces, title, xlabel, ylabel, filename, x_min):
    # Calculate R² correlation factor
    x = np.array(x_data)
    y = np.array(y_data)
    coefficients = np.polyfit(x, y, 1)  # Linear fit (degree 1)
    polynomial = np.poly1d(coefficients)
    y_pred = polynomial(x)
    ss_tot = np.sum((y - np.mean(y))**2)
    ss_res = np.sum((y - y_pred)**2)
    r_squared = 1 - (ss_res / ss_tot)
    print( f"x_data: {x_data}, y_data: {y_data}, r_squared: {r_squared:.3f}")

    # Create the scatter plot
    plt.figure(figsize=(10, 6))
    plt.scatter(x_data, y_data, color='blue', s=100)

    # Add labels for each point
    for i, province in enumerate(provinces):
        plt.text(x_data[i] - 0.3, y_data[i] - 0.05, province, fontsize=9, ha='right', va='bottom')

    # Plot the regression line
    plt.plot(x, y_pred, color='red', linestyle='--', label=f'Regression Line (R² = {r_squared:.3f})')

    # Set labels and title
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)

    # Set x-axis minimum
    plt.xlim(x_min, max(x_data) + 1)

    # Add grid
    plt.grid(True, linestyle='--', alpha=0.7)

    # Add URLs at the bottom
    plt.figtext(0.5, 0.02, "\n".join(urls), ha='center', fontsize=8, wrap=True)

    # Add legend
    plt.legend()

    # Adjust layout to prevent clipping of text
    plt.tight_layout(rect=[0, 0.1, 1, 1])

    # Save the plot
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    plt.close()

# Generate the four plots
create_scatter_plot(
    overall_obesity_rates, overall_life_expectancies, provinces,
    "Obesity Rate and Life Expectancy by Province",
    "Obesity Rate (%)", "Life Expectancy (2023)", "overall.png", 20
)
create_scatter_plot(
    male_obesity_rates, male_life_expectancies, provinces,
    "Obesity Rate and Life Expectancy of Males by Province",
    "Obesity Rate of Males (%)", "Life Expectancy of Males (2023)", "male.png", 20
)
create_scatter_plot(
    female_obesity_rates, female_life_expectancies, provinces,
    "Obesity Rate and Life Expectancy of Females by Province",
    "Obesity Rate of Females (%)", "Life Expectancy of Females (2023)", "female.png", 20
)

create_scatter_plot(
    alcohol_use, overall_life_expectancies, provinces,
    "12-Month Alcohol Use and Life Expectancy by Province",
    "12-Month Alcohol Use (%)", "Life Expectancy (2023)", "alcohol.png", 65
)

create_scatter_plot(
    alcohol_use, male_life_expectancies, provinces,
    "12-Month Alcohol Use and Life Expectancy by Province",
    "12-Month Alcohol Use (%)", "Life Expectancy of Males (2023)", "alcoholmales.png", 65
)
