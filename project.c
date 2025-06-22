#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Expense {
    char category[50];
    float amount;
    char date[15];
    char note[100];
};

#define MAX_EXPENSES 100
struct Expense expenses[MAX_EXPENSES];
int count = 0;

void loadExpenses() {
    FILE *file = fopen("expenses.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %f %s %[^\n]", expenses[count].category, &expenses[count].amount, expenses[count].date, expenses[count].note) != EOF) {
            count++;
        }
        fclose(file);
    }
}

void saveExpenseToFile(struct Expense e) {
    FILE *file = fopen("expenses.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %.2f %s %s\n", e.category, e.amount, e.date, e.note);
        fclose(file);
    }
}

void addExpense() {
    if (count >= MAX_EXPENSES) {
        printf("Cannot add more expenses. Limit reached.\n");
        return;
    }

    struct Expense e;
    printf("\nEnter Category (Food, Travel, etc.): ");
    scanf("%s", e.category);
    printf("Enter Amount: ");
    scanf("%f", &e.amount);
    printf("Enter Date (DD/MM/YYYY): ");
    scanf("%s", e.date);
    getchar();
    printf("Enter Note (e.g., lunch, petrol): ");
    fgets(e.note, sizeof(e.note), stdin);
    e.note[strcspn(e.note, "\n")] = '\0';

    expenses[count++] = e;
    saveExpenseToFile(e);

    printf("Expense added successfully.\n");
}

void viewExpenses() {
    if (count == 0) {
        printf("No expenses to show.\n");
        return;
    }

    printf("\nAll Recorded Expenses:\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%s | %.2f | %s | Note: %s\n", expenses[i].date, expenses[i].amount, expenses[i].category, expenses[i].note);
    }
    printf("----------------------------------------------------------\n");
}

void sortExpensesByAmount(int ascending) {
    struct Expense temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if ((ascending && expenses[j].amount > expenses[j + 1].amount) ||
                (!ascending && expenses[j].amount < expenses[j + 1].amount)) {
                temp = expenses[j];
                expenses[j] = expenses[j + 1];
                expenses[j + 1] = temp;
            }
        }
    }

    printf("\nExpenses Sorted by Amount (%s):\n", ascending ? "Low to High" : "High to Low");
    viewExpenses();
}

void calculateBalance(float income) {
    float total = 0;
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
    }

    float savings = income - total;
    printf("\nTotal Income: %.2f\n", income);
    printf("Total Expenses: %.2f\n", total);
    printf("Remaining Balance: %.2f\n", savings);

    if (savings < 0) {
        printf("Warning: You have spent more than your income.\n");
    }
}

void deleteExpenseByDate() {
    char targetDate[15];
    int found = 0;
    struct Expense e;

    printf("Enter the date (DD/MM/YYYY) of the expense to delete: ");
    scanf("%s", targetDate);

    FILE *file = fopen("expenses.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(file, "%s %f %s %[^\n]", e.category, &e.amount, e.date, e.note) != EOF) {
        if (strcmp(e.date, targetDate) != 0) {
            fprintf(tempFile, "%s %.2f %s %s\n", e.category, e.amount, e.date, e.note);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("expenses.txt");
    rename("temp.txt", "expenses.txt");

    if (found) {
        printf("Expense on %s deleted successfully.\n", targetDate);
        count = 0;
        loadExpenses();
    } else {
        printf("No expense found on that date.\n");
    }
}

int main() {
    int choice;
    float income;

    // Password Protection
    char password[20];
    printf("Enter Password to access the Expense Tracker: ");
    scanf("%s", password);
    if (strcmp(password, "admin123") != 0) {
        printf("Wrong Password! Access Denied.\n");
        return 0;
    }

    printf("Access Granted.\n");
    printf("Enter Monthly Income: ");
    scanf("%f", &income);

    loadExpenses();

    do {
        printf("\nMenu:\n");
        printf("1. Add Expense\n");
        printf("2. View All Expenses\n");
        printf("3. Check Balance & Savings\n");
        printf("4. Sort Expenses (Low to High)\n");
        printf("5. Sort Expenses (High to Low)\n");
        printf("6. Delete Expense by Date\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: calculateBalance(income); break;
            case 4: sortExpensesByAmount(1); break;
            case 5: sortExpensesByAmount(0); break;
            case 6: deleteExpenseByDate(); break;
            case 7: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);

    return 0;
}
