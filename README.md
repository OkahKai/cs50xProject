# E-Commerce System in C

#### Video Demo: https://youtu.be/d62b7U8uzpw?si=fMn0GXDL6RjfjfBT

#### Description:
The E-Commerce System in C is a command-line application designed to simulate an online store, where users can browse products, add them to their shopping cart, and proceed to checkout. This project is implemented in the C programming language and includes functionality such as user registration, login, product browsing, and order management. The system also supports loyalty points for returning users, adding a personalized touch to the shopping experience.

The application stores data in text files, allowing for persistent storage of user information, product details, and past orders. The user can interact with the program via a text-based interface, where they can perform actions like browsing products, managing their shopping cart, viewing past orders, and completing their purchases.

### Features:
- **User Registration and Login**: Users can register a new account or log in with an existing one. The system checks for valid credentials and allows access to user-specific features.
- **Product Browsing**: Users can view a list of available products, each with a unique ID, name, price, and stock quantity.
- **Cart Management**: Users can add products to their cart, specify quantities, and view the contents of their cart at any time.
- **Checkout and Loyalty Program**: Users can complete their purchase, with loyalty points applied to provide discounts. For every $1 spent, users earn loyalty points, which can later be redeemed for discounts.
- **Past Orders**: Users can view their past orders, which are stored in a text file for easy access.

### Design Choices:
The project was designed to focus on simplicity and clarity in code structure. The program uses basic file handling for persistent data storage, making it easy to load and save information.

- **File-Based Storage**:I chose text files (`products.txt` and `users.txt`) for storing data about products and users. This choice makes the system easier to maintain, while allowing the user data to be updated with every interaction.

- **Array-Based Data Management**: For simplicity, products and users are stored in static arrays, limiting the program to a fixed number of entries. This keeps memory usage predictable but also places a cap on scalability.

- **Loyalty Program**: To enhance user experience, we implemented a loyalty system. For every dollar spent, the user earns loyalty points, which can be redeemed for discounts. This adds a touch of gamification to the purchasing experience, encouraging users to engage with the system more frequently.

### Files in this Project:
- **ecommerce.c**: This is the main source code file. It contains the implementation of all features, including product browsing, adding items to the cart, checkout, and order management. It also includes functions for user registration and login.

- **products.txt**: This text file stores the product data, including the product ID, name, price, and available stock. It is read by the program at startup and updated whenever a product is added or sold.

- **users.txt**: This text file stores the user data, including the username, password, and loyalty points. It is read at startup and written to whenever a user registers or makes a purchase.

- **orders.txt**: This text file stores the user's past orders, including product IDs and quantities purchased. It provides a way for users to view their purchase history.
