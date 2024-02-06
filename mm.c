#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses/ncurses.h>

// Define a structure to represent a movie
struct Movie {
    char title[50];
    char genre[20];
    int year;
    int rating; // New field for movie rating
    char status[20]; // New field for movie status
};

// Function prototypes
void addMovie();
void displayMovies();
void searchMovie();

// Function to display menu and handle arrow key input
int showMenu(const char *options[], int numOptions);

int main() {
    int choice;

    initscr(); // Initialize ncurses
    keypad(stdscr, TRUE); // Enable arrow key input
    noecho(); // Do not display input characters

    do {
        clear(); // Clear the screen

        const char *menuOptions[] = {
            "Add a movie",
            "Display all movies",
            "Search for a movie",
            "Exit"
        };

        choice = showMenu(menuOptions, 4);

        switch (choice) {
            case 0:
                addMovie();
                break;
            case 1:
                displayMovies();
                break;
            case 2:
                searchMovie();
                break;
            case 3:
                printw("Exiting the program. Goodbye!\n");
                break;
            default:
                printw("Invalid choice. Please try again.\n");
        }

        refresh();
        getch(); // Wait for a key press
    } while (choice != 3);

    endwin(); // End ncurses

    return 0;
}

int showMenu(const char *options[], int numOptions) {
    int highlight = 0;
    int choice;
    int c;

    while (1) {
        clear(); // Clear the screen

        for (int i = 0; i < numOptions; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // Highlight the selected option
            }
            printw("%s\n", options[i]);
            attroff(A_REVERSE); // Turn off highlighting
        }

        // Get user input
        c = getch();

        switch (c) {
            case KEY_UP:
                highlight = (highlight - 1 + numOptions) % numOptions;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % numOptions;
                break;
            case 10: // Enter key
                choice = highlight;
                return choice;
            default:
                break;
        }
    }
}

void addMovie() {
    struct Movie newMovie;

    printw("Enter movie title: ");
    refresh();
    scanw(" %[^\n]", newMovie.title); //Enter will move the program forward

    printw("Enter movie genre: ");
    refresh();
    scanw(" %[^\n]", newMovie.genre);

    printw("Enter movie release year: ");
    refresh();
    scanw("%d", &newMovie.year);

    printw("Enter movie rating (1-10): ");
    refresh();
    scanw("%d", &newMovie.rating);

    printw("Select movie status:\n");
    const char *statusOptions[] = {
        "Plan to watch",
        "Watched",
        "Will rewatch"
    };
    
    int statusChoice = showMenu(statusOptions, 3);

    switch (statusChoice) {
        case 0:
            strcpy(newMovie.status, "Plan to watch");
            break;
        case 1:
            strcpy(newMovie.status, "Watched");
            break;
        case 2:
            strcpy(newMovie.status, "Will rewatch");
            break;
        default:
            printw("Invalid status choice. Setting status to 'Unknown'.\n");
            strcpy(newMovie.status, "Unknown");
    }

    FILE *file = fopen("movies.dat", "ab");
    if (file == NULL) {
        printw("Error opening file for writing.\n");
        return;
    }

    fwrite(&newMovie, sizeof(struct Movie), 1, file);

    fclose(file);

    printw("Movie added successfully!\n");
}

void displayMovies() {
    FILE *file = fopen("movies.dat", "rb");
    if (file == NULL) {
        printw("Error opening file for reading.\n");
        return;
    }

    struct Movie currentMovie;

    printw("\nAll Movies:\n");

    while (fread(&currentMovie, sizeof(struct Movie), 1, file) == 1) {
        printw("Title: %s\n", currentMovie.title);
        printw("Genre: %s\n", currentMovie.genre);
        printw("Year: %d\n", currentMovie.year);
        printw("Rating: %d/10 bananas\n", currentMovie.rating);
        printw("Status: %s\n", currentMovie.status);
        printw("\n");
    }

    fclose(file);
}

void searchMovie() {
    FILE *file = fopen("movies.dat", "rb");
    if (file == NULL) {
        printw("Error opening file for reading.\n");
        return;
    }

    char searchTitle[50];
    printw("Enter the title to search: ");
    refresh();
    scanw(" %[^\n]", searchTitle);

    struct Movie currentMovie;
    int found = 0;

    while (fread(&currentMovie, sizeof(struct Movie), 1, file) == 1) {
        if (strcmp(currentMovie.title, searchTitle) == 0) {
            printw("Movie found!\n");
            printw("Title: %s\n", currentMovie.title);
            printw("Genre: %s\n", currentMovie.genre);
            printw("Year: %d\n", currentMovie.year);
            printw("Rating: %d/10 bananas\n", currentMovie.rating);
            printw("Status: %s\n", currentMovie.status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printw("Movie not found.\n");
    }

    fclose(file);
}
