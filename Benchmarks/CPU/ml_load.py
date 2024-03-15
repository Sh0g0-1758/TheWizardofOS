import numpy as np
from sklearn.linear_model import LinearRegression
def ml_load():
    # Define the range of samples and features
    min_samples, max_samples = 5, 200
    min_features, max_features = 1, 5

    # Randomly select number of samples and features
    num_samples = np.random.randint(min_samples, max_samples + 1)
    num_features = np.random.randint(min_features, max_features + 1)

    # Generate random data
    X = np.random.rand(num_samples, num_features)
    y = np.random.rand(num_samples)

    # Create a linear regression model
    model = LinearRegression()

    # Train the model
    model.fit(X, y)

    # Generate random test data
    num_test_samples = np.random.randint(1, 10)
    X_test = np.random.rand(num_test_samples, num_features)

    # Predict using the trained model
    predictions = model.predict(X_test)
    return predictions