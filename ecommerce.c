#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 100  // Maximum number of products
#define MAX_USERS 50      // Maximum number of users
#define MAX_CART_ITEMS 10 // Maximum number of cart items

// Product structure
typedef struct
{
    int id;        // Product ID
    char name[50]; // Product name
    double price;  // Product price
    int stock;     // Product stock
} Product;

// User structure
typedef struct
{
    char username[50]; // User's username
    char password[50]; // User's password
    int loyalty;       // Loyalty points for the user
} User;

// Global data
Product products[MAX_PRODUCTS];     // Array to store all products
User users[MAX_USERS];              // Array to store all users
int productCount = 0;               // Counter for the number of products
int userCount = 0;                  // Counter for the number of users
int cart[MAX_CART_ITEMS];           // Array to store product IDs in the cart
int cartQuantities[MAX_CART_ITEMS]; // Array to store quantities of products in the cart
int cartItemCount = 0;              // Number of items currently in the cart
int currentUserIndex = -1;          // Index of the current logged-in user

// Function declarations
void loadProducts();
void loadUsers();
void saveProducts();
void saveUsers();
int loginUser(char *username, char *password);
int registerUser(char *username, char *password);
void browseProducts();
void addToCart(int productId, int quantity);
void viewCart();
void checkout();
void viewPastOrders(char *username);
void printMenu();
void saveOrder(char *username);

// Trim leading and trailing spaces from a string
void trim(char *str)
{
    char *end;

    // Remove leading spaces
    while (isspace((unsigned char) *str))
        str++;

    // If string is empty, return
    if (*str == 0)
        return;

    // Remove trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;

    *(end + 1) = 0; // Null-terminate the string
}

// Load products from a file
void loadProducts()
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
    {
        printf("Error opening products file.\n");
        return;
    }

    // Read and discard the header line
    char header[256];
    fgets(header, sizeof(header), file);

    // Read product data from file
    while (fscanf(file, "%d, %49[^,], %lf, %d", &products[productCount].id,
                  products[productCount].name, &products[productCount].price,
                  &products[productCount].stock) == 4)
    {
        productCount++; // Increment the product count
    }

    fclose(file); // Close the file after reading
}

// Load users from a file
void loadUsers()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error opening users file.\n");
        return;
    }

    char line[150];
    while (fgets(line, sizeof(line), file))
    {
        // Read user data from file
        sscanf(line, "%49[^,], %49[^,], %d", users[userCount].username, users[userCount].password,
               &users[userCount].loyalty);
        userCount++; // Increment the user count
    }
    fclose(file); // Close the file after reading
}

// Save products to a file
void saveProducts()
{
    FILE *file = fopen("products.txt", "w");
    if (file == NULL)
    {
        printf("Error saving products file.\n");
        return;
    }

    // Write each product's data to the file
    for (int i = 0; i < productCount; i++)
    {
        fprintf(file, "%d, %s, %.2f, %d\n", products[i].id, products[i].name, products[i].price,
                products[i].stock);
    }
    fclose(file); // Close the file after saving
}

// Save users to a file
void saveUsers()
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error saving users file.\n");
        return;
    }

    // Write each user's data to the file
    for (int i = 0; i < userCount; i++)
    {
        fprintf(file, "%s, %s, %d\n", users[i].username, users[i].password, users[i].loyalty);
    }
    fclose(file); // Close the file after saving
}

// Authenticate a user based on username and password
int loginUser(char *username, char *password)
{
    trim(username); // Remove any leading/trailing spaces
    trim(password); // Remove any leading/trailing spaces
    for (int i = 0; i < userCount; i++)
    {
        // Check if username and password match
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            currentUserIndex = i; // Set the current user index
            return i;             // Return the user index
        }
    }
    return -1; // Return -1 if the user was not found
}

// Register a new user with username and password
int registerUser(char *username, char *password)
{
    if (userCount >= MAX_USERS)
    {
        printf("Maximum user limit reached.\n");
        return -1;
    }

    // Check if the username already exists
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Username already exists.\n");
            return -1;
        }
    }

    // Add new user
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].loyalty = 0; // Initialize loyalty points to 0
    userCount++;                  // Increment the user count
    saveUsers();                  // Save the updated users to the file
    printf("User registered successfully!\n");
    return userCount - 1; // Return the index of the newly registered user
}

// Display the list of available products
void browseProducts()
{
    printf("\nAvailable Products:\n");
    for (int i = 0; i < productCount; i++)
    {
        // Display each product's information
        printf("%d. %s | Price: %.2f | Stock: %d\n", products[i].id, products[i].name,
               products[i].price, products[i].stock);
    }
}

// Add a product to the cart
void addToCart(int productId, int quantity)
{
    if (cartItemCount >= MAX_CART_ITEMS)
    {
        printf("Cart is full!\n");
        return;
    }
    if (productId < 0 || productId >= productCount || quantity <= 0)
    {
        printf("Invalid product ID or quantity.\n");
        return;
    }
    if (products[productId].stock < quantity)
    {
        printf("Not enough stock.\n");
        return;
    }

    cart[cartItemCount] = productId;          // Add product ID to the cart
    cartQuantities[cartItemCount] = quantity; // Add quantity to the cart
    cartItemCount++;                          // Increment the cart item count
    products[productId].stock -= quantity;    // Deduct stock from the product
    printf("Added %d of %s to your cart.\n", quantity, products[productId].name);
}

// View the current items in the cart
void viewCart()
{
    if (cartItemCount == 0)
    {
        printf("Your cart is empty.\n");
        return;
    }

    printf("\nYour Cart:\n");
    for (int i = 0; i < cartItemCount; i++)
    {
        // Display the details of each item in the cart
        printf("%d. %s | Quantity: %d | Price: %.2f | Total: %.2f\n", cart[i] + 1,
               products[cart[i]].name, cartQuantities[i], products[cart[i]].price,
               products[cart[i]].price * cartQuantities[i]);
    }
}

// Finalize the purchase and save the order
void checkout()
{
    if (cartItemCount == 0)
    {
        printf("Your cart is empty.\n");
        return;
    }

    double totalAmount = 0;
    for (int i = 0; i < cartItemCount; i++)
    {
        totalAmount += products[cart[i]].price * cartQuantities[i];
    }

    // Apply loyalty points discount (100 points = 5% off)
    double discount = (users[currentUserIndex].loyalty / 100) * 0.05 * totalAmount;
    totalAmount -= discount;

    printf("\nCheckout:\nTotal: %.2f\nDiscount: %.2f\nAmount to pay: %.2f\n",
           totalAmount + discount, discount, totalAmount);

    // Update user's loyalty points
    users[currentUserIndex].loyalty +=
        (int) (totalAmount / 10);                // Earn 10 loyalty points for every $1 spent
    saveUsers();                                 // Save updated user data
    saveOrder(users[currentUserIndex].username); // Save the order
    cartItemCount = 0;                           // Clear the cart after checkout
}

// View the user's past orders
void viewPastOrders(char *username)
{
    // Example implementation: print past orders from a file or a database
    printf("Past orders for %s:\n", username);

    // Assuming orders are stored in a file named 'orders.txt'
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL)
    {
        printf("Error opening orders file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char orderUser[50];
        sscanf(line, "%49[^,]", orderUser);
        if (strcmp(orderUser, username) == 0)
        {
            printf("%s", line); // Print the order details
        }
    }

    fclose(file);
}

// Save an order for the user
void saveOrder(char *username)
{
    FILE *file = fopen("orders.txt", "a");
    if (file == NULL)
    {
        printf("Error saving orders file.\n");
        return;
    }

    // Write order data to the file
    fprintf(file, "%s, ", username);
    for (int i = 0; i < cartItemCount; i++)
    {
        fprintf(file, "%d (x%d), ", products[cart[i]].id, cartQuantities[i]);
    }
    fprintf(file, "Total: %.2f\n", products[cart[0]].price * cartQuantities[0]);
    fclose(file);
}

// Display the main menu
void printMenu()
{
    printf("\n1. Browse Products\n");
    printf("2. Add to Cart\n");
    printf("3. View Cart\n");
    printf("4. Checkout\n");
    printf("5. View Past Orders\n");
    printf("6. Exit\n");
}

int main()
{
    loadProducts();
    loadUsers();

    char username[50];
    char password[50];
    int choice;

    printf("Welcome to the E-commerce system!\n");

    while (1)
    {
        printf("\nEnter your username: ");
        fgets(username, sizeof(username), stdin);
        trim(username); // Remove any leading/trailing spaces

        printf("Enter your password: ");
        fgets(password, sizeof(password), stdin);
        trim(password);

        int loggedIn = loginUser(username, password);
        if (loggedIn != -1)
        {
            printf("Welcome, %s!\n", users[loggedIn].username);
            break; // Exit the loop after successful login
        }
        else
        {
            printf("Invalid credentials. Would you like to register? (y/n): ");
            char response;
            scanf("%c", &response);
            getchar(); // To consume the trailing newline character
            if (response == 'y' || response == 'Y')
            {
                printf("Enter your new username: ");
                fgets(username, sizeof(username), stdin);
                trim(username);
                printf("Enter your new password: ");
                fgets(password, sizeof(password), stdin);
                trim(password);
                registerUser(username, password);
            }
        }
    }

    // Main menu loop
    while (1)
    {
        printMenu(); // Print the main menu
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the trailing newline character

        switch (choice)
        {
            case 1:
                browseProducts();
                break;
            case 2:
            {
                        int productId, quantity;
                printf("Enter product ID: ");
                scanf("%d", &productId);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                addToCart(productId - 1, quantity); // Adjust product ID for array index
                break;
            }
            case 3:
                viewCart();
                break;
            case 4:
                checkout();
                break;
            case 5:
                viewPastOrders(users[currentUserIndex].username);
                break;
            case 6:
                saveProducts(); // Save products before exiting
                saveUsers();    // Save users before exiting
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
printf("This was CS50");
    return 0;
}
