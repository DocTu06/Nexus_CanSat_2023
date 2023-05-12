import numpy as np
import matplotlib.pyplot as plt
from numpy.linalg import inv


def least_sq(sample_spectrum, components):
    # sample_spectrum (unknown spectrum): array of w values. components (known spectra): array of n (number of
    # components) columns with w values. This def returns an array of n values. Each value is the similarity score
    # for the sample_spectrum and a component spectrum.
    similarity = np.dot(inv(np.dot(components, components.T)), np.dot(components, sample_spectrum))
    return similarity


def gauss(x, mu, sigma, intensity=1):
    # x is an array
    # mu is the expected value
    # sigma is the square root of the variance
    # intensity is a multiplication factor
    # This def returns the Gaussian function of x
    return intensity / (sigma * np.sqrt(2 * np.pi)) * (np.exp(-0.5 * ((x - mu) / sigma) ** 2))

# X-axis (Wavelengths)
x_range = np.linspace(100, 200, 1000)

# Let's create three different components
# Component A
mu_a1 = 120
sigma_a1 = 2
intensity_a1 = 1
mu_a2 = 185
sigma_a2 = 2
intensity_a2 = 0.2
gauss_a = gauss(x_range, mu_a1, sigma_a1, intensity_a1) + gauss(x_range, mu_a2, sigma_a2, intensity_a2)
# Component B
mu_b = 150
sigma_b = 15
intensity_b = 1
gauss_b = gauss(x_range, mu_b, sigma_b, intensity_b)
# Component C
mu_c1 = 110
sigma_c1 = 2
intensity_c1 = 0.05
mu_c2 = 160
sigma_c2 = 10
intensity_c2 = 1
gauss_c = gauss(x_range, mu_c1, sigma_c1, intensity_c1) + gauss(x_range, mu_c2, sigma_c2, intensity_c2)
# Component D
mu_d = 140
sigma_d = 4
intensity_d = 1
gauss_d = gauss(x_range, mu_d, sigma_d, intensity_d)
# Spectra normalization:
component_a = gauss_a / np.max(gauss_a)
component_b = gauss_b / np.max(gauss_b)
component_c = gauss_c / np.max(gauss_c)
component_d = gauss_d / np.max(gauss_d)

# What concentrations we want them to have in our mixture:
c_a = 0.781
c_b = 0.219
c_c = 0.006
c_d = 0.004
# Let's build the spectrum to be studied: The mixture spectrum
query_spectra = c_a * component_a + c_b * component_b + c_c * component_c + c_d * component_d
# Let's add it some noise for a bit of realism:
query_spectra = query_spectra + np.random.normal(0, 0.02, len(x_range))
# Generate the components matrix or K matrix
components = np.array([component_a, component_b, component_c, component_d])
# Apply Least squares
cs = least_sq(query_spectra, components)
# And plot the result:
plt.plot(x_range, query_spectra, color='black', label='Mix spectrum')  # Plot the unknown spectrum
plt.plot(x_range, np.dot(cs, components), color='red', linewidth=2, label='Calculation')  # Plot the calculated spectrum
gas_list = ['Nitrogen', 'Oxygen', 'Argon', 'Other gases']
for i in np.arange(len(cs)):
    plt.plot(x_range, cs[i] * components[i], label='' + str(gas_list[i]) + ' =     ' + str(np.round(cs[i], 3)))
plt.title('Mixture spectrum and calculated components', fontsize=15)
plt.xlabel('Wavelength', fontsize=15)
plt.ylabel('Intensity', fontsize=15)
plt.legend()
plt.show()