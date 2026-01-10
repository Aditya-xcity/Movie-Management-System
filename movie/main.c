#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_MOVIES 2      
#define MAX_BOOKINGS 100
#define MAX_COMING_SOON 10
#define MAX_PREMIUM_SEATS 25
#define MAX_REGULAR_SEATS 75

typedef struct {
    char name[50];
    int code;
    char description[100];
    int premium_price;
    int regular_price;
    char date[20];
    char time[20];
    int hall_number;
    bool premium_seats[MAX_PREMIUM_SEATS];
    bool regular_seats[MAX_REGULAR_SEATS];
} Movie;

typedef struct {
    char name[50];
    int code;
    char description[100];
} ComingSoon;

typedef struct {
    int booking_id;
    char customer_name[50];
    unsigned long long mobile_number;
    int movie_code;
    char movie_name[50];
    int hall_number;
    int premium_tickets;
    int regular_tickets;
    char premium_seats[MAX_PREMIUM_SEATS][4];  
    char regular_seats[MAX_REGULAR_SEATS][4];  
    float total_price;
    float discount;  
    float tax;       
    char payment_method[20];
    char purchase_date[20];
    char purchase_time[20];
} Booking;

Movie movies[MAX_MOVIES] = {0};
ComingSoon coming_soon[MAX_COMING_SOON] = {0};
Booking bookings[MAX_BOOKINGS] = {0};

int num_movies = 0;
int num_coming_soon = 0;
int num_bookings = 0;
int last_booking_id = 1000;

void initialize_seats();
void login_panel();
void admin_panel();
void customer_menu();
void save_movies();
void load_movies();
void save_bookings();
void load_bookings();
void add_movie();
void book_ticket(int movie_index);
void show_bill(Booking *booking);
void customer_view_bookings();
void admin_view_bookings();
void admin_view_movie_records();
void search_booking_by_id();
void payment_gateway(Booking *booking);
void generate_booking_id(Booking *booking);
void display_seat_availability(int movie_index);
bool book_seats(int movie_index, Booking *booking);

int main() {
    initialize_seats();
    load_movies();
    load_bookings(); 
    login_panel();
    return 0;
}

void initialize_seats() {
    for (int i = 0; i < MAX_MOVIES; i++) {
        for (int j = 0; j < MAX_PREMIUM_SEATS; j++) {
            movies[i].premium_seats[j] = false;
        }
        for (int j = 0; j < MAX_REGULAR_SEATS; j++) {
            movies[i].regular_seats[j] = false;
        }
    }
}

void generate_booking_id(Booking *booking) {
    last_booking_id++;
    booking->booking_id = last_booking_id;
}

void login_panel() {
    int choice;
    char username[20], password[20];

    printf("\n\t=== CINEMA MANAGEMENT SYSTEM ===\n");
    printf("\n\t1. Admin Login");
    printf("\n\t2. Customer Menu");
    printf("\n\t3. Exit");
    printf("\n\tEnter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("\n\tUsername: ");
            scanf("%19s", username);
            printf("\tPassword: ");
            scanf("%19s", password);
            
            if(strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                admin_panel();
            } else {
                printf("\n\tInvalid credentials!");
                login_panel();
            }
            break;
        case 2:
            customer_menu();
            break;
        case 3:
            save_movies();
            save_bookings();
            printf("Good Bye!");
            exit(0);
        default:
            printf("\n\tInvalid choice!");
            login_panel();
    }
}

void admin_panel() {
    int choice;
    printf("\n\t=== ADMIN PANEL ===");
    printf("\n\t1. Add/Change Movie");
    printf("\n\t2. Add Coming Soon Movie");
    printf("\n\t3. View All Bookings");
    printf("\n\t4. View Movie Records");
    printf("\n\t5. Search Booking by ID");
    printf("\n\t6. Back to Main");
    printf("\n\tEnter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            add_movie();
            admin_panel();
            break;
        case 2:
            if(num_coming_soon < MAX_COMING_SOON) {
                printf("\n\tEnter coming soon movie name: ");
                scanf("%49s", coming_soon[num_coming_soon].name);
                printf("\tEnter description: ");
                scanf("%99s", coming_soon[num_coming_soon].description);
                coming_soon[num_coming_soon].code = 100 + num_coming_soon;
                num_coming_soon++;
            } else {
                printf("\n\tComing soon list full!");
            }
            admin_panel();
            break;
        case 3:
            admin_view_bookings();
            admin_panel();
            break;
        case 4:
            admin_view_movie_records();
            admin_panel();
            break;
        case 5:
            search_booking_by_id();
            admin_panel();
            break;
        case 6:
            login_panel();
            break;
        default:
            printf("\n\tInvalid choice!");
            admin_panel();
    }
}

void add_movie() {
    int hall_choice;
    printf("\n\tSelect hall (1 or 2): ");
    scanf("%d", &hall_choice);
    getchar(); 

    if (hall_choice < 1 || hall_choice > 2) {
        printf("\n\tInvalid hall selection!");
        return;
    }

    int movie_index = hall_choice - 1; 

    
    for (int j = 0; j < MAX_PREMIUM_SEATS; j++) {
        movies[movie_index].premium_seats[j] = false;
    }
    for (int j = 0; j < MAX_REGULAR_SEATS; j++) {
        movies[movie_index].regular_seats[j] = false;
    }

    movies[movie_index].code = hall_choice;
    movies[movie_index].hall_number = hall_choice;

    printf("\n\tEnter movie name: ");
    fgets(movies[movie_index].name, sizeof(movies[movie_index].name), stdin);
    movies[movie_index].name[strcspn(movies[movie_index].name, "\n")] = '\0'; 

    printf("\tEnter description: ");
    fgets(movies[movie_index].description, sizeof(movies[movie_index].description), stdin);
    movies[movie_index].description[strcspn(movies[movie_index].description, "\n")] = '\0'; 

    printf("\tEnter date (DD-MM-YYYY): ");
    scanf("%19s", movies[movie_index].date);
    printf("\tEnter time (HH:MM): ");
    scanf("%19s", movies[movie_index].time);
    printf("\tEnter premium seat price: ");
    scanf("%d", &movies[movie_index].premium_price);
    printf("\tEnter regular seat price: ");
    scanf("%d", &movies[movie_index].regular_price);

    if (num_movies < hall_choice) {
        num_movies = hall_choice;
    }

    save_movies();
    printf("\n\tMovie added/changed in Hall %d successfully!\n", hall_choice);
}

void admin_view_bookings() {
    if (num_bookings == 0) {
        printf("\n\tNo bookings found.\n");
        return;
    }

    printf("\n\t=== ALL BOOKINGS ===\n");
    for (int i = 0; i < num_bookings; i++) {
        Booking *b = &bookings[i];
        printf("\n\tBooking ID: %d", b->booking_id);
        printf("\n\tCustomer Name: %s", b->customer_name);
        printf("\n\tMobile Number: %llu", b->mobile_number);
        printf("\n\tMovie: %s (Hall %d)", b->movie_name, b->hall_number);
        printf("\n\tPremium Tickets: %d", b->premium_tickets);
        printf("\n\tRegular Tickets: %d", b->regular_tickets);

        if (b->premium_tickets > 0) {
            printf("\n\tPremium Seats: ");
            for (int j = 0; j < b->premium_tickets; j++) {
                printf("%s", b->premium_seats[j]);
                if (j < b->premium_tickets - 1) printf(", ");
            }
        }

        if (b->regular_tickets > 0) {
            printf("\n\tRegular Seats: ");
            for (int j = 0; j < b->regular_tickets; j++) {
                printf("%s", b->regular_seats[j]);
                if (j < b->regular_tickets - 1) printf(", ");
            }
        }

        printf("\n\tDiscount: Rs.%.2f", b->discount);
        printf("\n\tTax: Rs.%.2f", b->tax);
        printf("\n\tTotal Paid: Rs.%.2f", b->total_price);
        printf("\n\tPayment Method: %s", b->payment_method);
        printf("\n\tPurchase Date: %s %s\n", b->purchase_date, b->purchase_time);
    }
}

void admin_view_movie_records() {
    printf("\n\t=== MOVIE RECORDS ===");
    for(int i = 0; i < num_movies; i++) {
        int premium_available = 0;
        int regular_available = 0;
        
        for (int j = 0; j < MAX_PREMIUM_SEATS; j++) {
            if (!movies[i].premium_seats[j]) premium_available++;
        }
        
        for (int j = 0; j < MAX_REGULAR_SEATS; j++) {
            if (!movies[i].regular_seats[j]) regular_available++;
        }
        
        printf("\n\t%d. %s (Hall %d)", movies[i].code, movies[i].name, movies[i].hall_number);
        printf("\n\t   Date: %s | Time: %s", movies[i].date, movies[i].time);
        
        printf("\n\t   Premium Seats (Rs.%d):", movies[i].premium_price);
        printf("\n\t     Total: %d | Booked: %d | Available: %d", 
              MAX_PREMIUM_SEATS, 
              MAX_PREMIUM_SEATS - premium_available,
              premium_available);
        
        printf("\n\t   Regular Seats (Rs.%d):", movies[i].regular_price);
        printf("\n\t     Total: %d | Booked: %d | Available: %d", 
              MAX_REGULAR_SEATS,
              MAX_REGULAR_SEATS - regular_available,
              regular_available);
        
        printf("\n\t   Total Tickets: %d | Booked: %d | Available: %d\n",
              MAX_PREMIUM_SEATS + MAX_REGULAR_SEATS,
              (MAX_PREMIUM_SEATS - premium_available) + (MAX_REGULAR_SEATS - regular_available),
              premium_available + regular_available);
    }
}

void search_booking_by_id() {
    int search_id;
    printf("\n\tEnter Booking ID to search: ");
    scanf("%d", &search_id);
    
    for(int i = 0; i < num_bookings; i++) {
        if(bookings[i].booking_id == search_id) {
            printf("\n\t=== BOOKING FOUND ===");
            printf("\n\tID: %d", bookings[i].booking_id);
            printf("\n\tCustomer: %s (%llu)", bookings[i].customer_name, bookings[i].mobile_number);
            printf("\n\tMovie: %s (Hall %d)", bookings[i].movie_name, bookings[i].hall_number);
            
            if (bookings[i].premium_tickets > 0) {
                printf("\n\tPremium: %d seats (", bookings[i].premium_tickets);
                for (int j = 0; j < bookings[i].premium_tickets; j++) {
                    printf("%s", bookings[i].premium_seats[j]);
                    if (j < bookings[i].premium_tickets - 1) printf(", ");
                }
                printf(")");
            }
            
            if (bookings[i].regular_tickets > 0) {
                printf("\n\tRegular: %d seats (", bookings[i].regular_tickets);
                for (int j = 0; j < bookings[i].regular_tickets; j++) {
                    printf("%s", bookings[i].regular_seats[j]);
                    if (j < bookings[i].regular_tickets - 1) printf(", ");
                }
                printf(")");
            }
            
            printf("\n\tAmount: Rs.%.2f", bookings[i].total_price);
            printf("\n\tBooked on: %s %s\n", 
                  bookings[i].purchase_date, bookings[i].purchase_time);
            return;
        }
    }
    printf("\n\tNo booking found with ID: %d\n", search_id);
}

void customer_menu() {
    int choice;
    printf("\n\t=== CUSTOMER MENU ===");
    printf("\n\t1. Now Showing");
    printf("\n\t2. Coming Soon");
    printf("\n\t3. Book Tickets");
    printf("\n\t4. My Bookings");
    printf("\n\t5. Back to Main");
    printf("\n\tEnter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("\n\t=== NOW SHOWING ===");
            for(int i = 0; i < num_movies; i++) {
                int premium_available = 0;
                int regular_available = 0;
                
                for (int j = 0; j < MAX_PREMIUM_SEATS; j++) {
                    if (!movies[i].premium_seats[j]) premium_available++;
                }
                
                for (int j = 0; j < MAX_REGULAR_SEATS; j++) {
                    if (!movies[i].regular_seats[j]) regular_available++;
                }
                
                printf("\n\t%d. %s (Hall %d)", i+1, movies[i].name, movies[i].hall_number);
                printf("\n\t   Date: %s | Time: %s", movies[i].date, movies[i].time);
                printf("\n\t   Premium: Rs.%d (%d available)", 
                      movies[i].premium_price, premium_available);
                printf("\n\t   Regular: Rs.%d (%d available)", 
                      movies[i].regular_price, regular_available);
            }
            customer_menu();
            break;
        case 2:
            printf("\n\t=== COMING SOON ===");
            for(int i = 0; i < num_coming_soon; i++) {
                printf("\n\t%s - %s", coming_soon[i].name, coming_soon[i].description);
            }
            customer_menu();
            break;
        case 3:
            if(num_movies > 0) {
                int movie_choice;
                printf("\n\tSelect movie (1-%d): ", num_movies);
                scanf("%d", &movie_choice);
                if(movie_choice > 0 && movie_choice <= num_movies) {
                    book_ticket(movie_choice-1);
                } else {
                    printf("\n\tInvalid selection!");
                }
            } else {
                printf("\n\tNo movies available!");
            }
            customer_menu();
            break;
        case 4:
            customer_view_bookings();
            customer_menu();
            break;
        case 5:
            login_panel();
            break;
        default:
            printf("\n\tInvalid choice!");
            customer_menu();
    }
}

void customer_view_bookings() {
    unsigned long long mobile;
    int found = 0;
    
    printf("\n\tEnter your mobile number: ");
    scanf("%llu", &mobile);
    
    printf("\n\t=== YOUR BOOKINGS ===");
    for(int i = 0; i < num_bookings; i++) {
        if(bookings[i].mobile_number == mobile) {
            found = 1;
            printf("\n\tBooking ID: %d", bookings[i].booking_id);
            printf("\n\tMovie: %s (Hall %d)", bookings[i].movie_name, bookings[i].hall_number);
            
            if (bookings[i].premium_tickets > 0) {
                printf("\n\tPremium: %d seats (", bookings[i].premium_tickets);
                for (int j = 0; j < bookings[i].premium_tickets; j++) {
                    printf("%s", bookings[i].premium_seats[j]);
                    if (j < bookings[i].premium_tickets - 1) printf(", ");
                }
                printf(")");
            }
            
            if (bookings[i].regular_tickets > 0) {
                printf("\n\tRegular: %d seats (", bookings[i].regular_tickets);
                for (int j = 0; j < bookings[i].regular_tickets; j++) {
                    printf("%s", bookings[i].regular_seats[j]);
                    if (j < bookings[i].regular_tickets - 1) printf(", ");
                }
                printf(")");
            }
            
            printf("\n\tAmount: Rs.%.2f", bookings[i].total_price);
            printf("\n\tBooked on: %s %s\n", 
                  bookings[i].purchase_date, bookings[i].purchase_time);
        }
    }
    
    if(!found) {
        printf("\n\tNo bookings found for this number!");
    }
}

void display_seat_availability(int movie_index) {
    printf("\n\t=== SEAT AVAILABILITY (Hall %d) ===", movies[movie_index].hall_number);
    printf("\n\tPremium Seats (P1-P%d):\n\t", MAX_PREMIUM_SEATS);
    for (int i = 0; i < MAX_PREMIUM_SEATS; i++) {
        printf("%c%02d ", movies[movie_index].premium_seats[i] ? 'X' : 'P', i+1);
        if ((i+1) % 10 == 0) printf("\n\t");
    }
    
    printf("\n\tRegular Seats (R1-R%d):\n\t", MAX_REGULAR_SEATS);
    for (int i = 0; i < MAX_REGULAR_SEATS; i++) {
        printf("%c%02d ", movies[movie_index].regular_seats[i] ? 'X' : 'R', i+1);
        if ((i+1) % 10 == 0) printf("\n\t");
    }
    printf("\n\t(X = Booked, P/R = Available)\n");
}

bool book_seats(int movie_index, Booking *booking) {
    int premium_count = 0;
    int regular_count = 0;
    bool temp_premium_seats[MAX_PREMIUM_SEATS] = {false};
    bool temp_regular_seats[MAX_REGULAR_SEATS] = {false};
    
    
    printf("\n\tEnter number of premium seats (0-%d): ", MAX_PREMIUM_SEATS);
    scanf("%d", &premium_count);
    
    if (premium_count > 0) {
        printf("\tSelect %d premium seat(s) (P1-P%d):\n", premium_count, MAX_PREMIUM_SEATS);
        for (int i = 0; i < premium_count; i++) {
            int seat_num;
            printf("\tSeat %d: P", i+1);
            scanf("%d", &seat_num);
            
            if (seat_num < 1 || seat_num > MAX_PREMIUM_SEATS) {
                printf("\tInvalid seat number! Must be between 1 and %d\n", MAX_PREMIUM_SEATS);
                return false;
            }
            
            if (movies[movie_index].premium_seats[seat_num-1] || temp_premium_seats[seat_num-1]) {
                printf("\tSeat P%d is already booked or selected!\n", seat_num);
                return false;
            }
            
            temp_premium_seats[seat_num-1] = true;
            sprintf(booking->premium_seats[i], "P%d", seat_num);
        }
    }
    
    
    printf("\n\tEnter number of regular seats (0-%d): ", MAX_REGULAR_SEATS);
    scanf("%d", &regular_count);
    
    if (regular_count > 0) {
        printf("\tSelect %d regular seat(s) (R1-R%d):\n", regular_count, MAX_REGULAR_SEATS);
        for (int i = 0; i < regular_count; i++) {
            int seat_num;
            printf("\tSeat %d: R", i+1);
            scanf("%d", &seat_num);
            
            if (seat_num < 1 || seat_num > MAX_REGULAR_SEATS) {
                printf("\tInvalid seat number! Must be between 1 and %d\n", MAX_REGULAR_SEATS);
                return false;
            }
            
            if (movies[movie_index].regular_seats[seat_num-1] || temp_regular_seats[seat_num-1]) {
                printf("\tSeat R%d is already booked or selected!\n", seat_num);
                return false;
            }
            
            temp_regular_seats[seat_num-1] = true;
            (booking->regular_seats[i], "R%d", seat_num);
        }
    }
    
    if (premium_count == 0 && regular_count == 0) {
        printf("\n\tYou must book at least one seat!\n");
        return false;
    }
    
    
    for (int i = 0; i < MAX_PREMIUM_SEATS; i++) {
        if (temp_premium_seats[i]) {
            movies[movie_index].premium_seats[i] = true;
        }
    }
    
    for (int i = 0; i < MAX_REGULAR_SEATS; i++) {
        if (temp_regular_seats[i]) {
            movies[movie_index].regular_seats[i] = true;
        }
    }
    
    booking->premium_tickets = premium_count;
    booking->regular_tickets = regular_count;
    
    return true;
}

void book_ticket(int movie_index) {
    
    int premium_available = 0;
    int regular_available = 0;

    for (int i = 0; i < MAX_PREMIUM_SEATS; i++) {
        if (!movies[movie_index].premium_seats[i]) premium_available++;
    }

    for (int i = 0; i < MAX_REGULAR_SEATS; i++) {
        if (!movies[movie_index].regular_seats[i]) regular_available++;
    }

    if (premium_available + regular_available == 0) {
        printf("\n\tSorry, this show is sold out!");
        return;
    }

    Booking new_booking = {0};
    getchar(); 
    printf("\n\tEnter your name: ");
    fgets(new_booking.customer_name, sizeof(new_booking.customer_name), stdin);
    new_booking.customer_name[strcspn(new_booking.customer_name, "\n")] = '\0'; 

    printf("\tEnter mobile number: ");
    scanf("%llu", &new_booking.mobile_number);

    
    display_seat_availability(movie_index);

    
    if (!book_seats(movie_index, &new_booking)) {
        printf("\n\tBooking failed. Please try again.\n");
        return;
    }

    
    new_booking.total_price = (new_booking.premium_tickets * movies[movie_index].premium_price) +
                              (new_booking.regular_tickets * movies[movie_index].regular_price);

    
    new_booking.discount = new_booking.total_price * 0.05;
    new_booking.total_price -= new_booking.discount;

    
    new_booking.tax = new_booking.total_price * 0.18;
    new_booking.total_price += new_booking.tax;

    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(new_booking.purchase_date, 20, "%d-%m-%Y", t);
    strftime(new_booking.purchase_time, 20, "%H:%M:%S", t);

    
    new_booking.movie_code = movies[movie_index].code;
    strcpy(new_booking.movie_name, movies[movie_index].name);
    new_booking.hall_number = movies[movie_index].hall_number;

    generate_booking_id(&new_booking);
    payment_gateway(&new_booking);
}

void payment_gateway(Booking *booking) {
    int choice;
    printf("\n\t=== PAYMENT ===");
    printf("\n\tTotal Amount: Rs.%.2f", booking->total_price);
    printf("\n\t1. Credit/Debit Card");
    printf("\n\t2. Mobile Payment");
    printf("\n\t3. Cancel Booking");
    printf("\n\tEnter choice: ");
    scanf("%d", &choice);

    if(choice == 3) {
        printf("\n\tBooking cancelled!");
        return;
    }

    
    if(choice == 1) {
        long card_number;
        printf("\tEnter card number: ");
        scanf("%ld", &card_number);
        strcpy(booking->payment_method, "Card");
    } else if(choice == 2) {
        long mobile_number;
        printf("\tEnter mobile number: ");
        scanf("%ld", &mobile_number);
        strcpy(booking->payment_method, "Mobile");
    }

    
    if(num_bookings < MAX_BOOKINGS) {
        bookings[num_bookings] = *booking;
        num_bookings++;
        save_bookings();
        save_movies();
        show_bill(booking);
    }
}

void show_bill(Booking *booking) {
    printf("\n\t=== BOOKING CONFIRMED ===");
    printf("\n\tBooking ID: %d", booking->booking_id);
    printf("\n\tName: %s", booking->customer_name);
    printf("\n\tMobile: %llu", booking->mobile_number);
    printf("\n\tMovie: %s (Hall %d)", booking->movie_name, booking->hall_number);
    printf("\n\tPurchase Date: %s %s", booking->purchase_date, booking->purchase_time);
    
    if (booking->premium_tickets > 0) {
        printf("\n\tPremium Tickets: %d", booking->premium_tickets);
        printf("\n\tSeats: ");
        for (int i = 0; i < booking->premium_tickets; i++) {
            printf("%s", booking->premium_seats[i]);
            if (i < booking->premium_tickets - 1) printf(", ");
        }
    }
    
    if (booking->regular_tickets > 0) {
        printf("\n\tRegular Tickets: %d", booking->regular_tickets);
        printf("\n\tSeats: ");
        for (int i = 0; i < booking->regular_tickets; i++) {
            printf("%s", booking->regular_seats[i]);
            if (i < booking->regular_tickets - 1) printf(", ");
        }
    }
    
    printf("\n\tDiscount: Rs.%.2f", booking->discount);
    printf("\n\tTax: Rs.%.2f", booking->tax);
    printf("\n\tTotal Paid: Rs.%.2f", booking->total_price);
    printf("\n\tPayment Method: %s\n", booking->payment_method);
    printf("\n\tThank you for your booking!\n");
}

void save_movies() {
    FILE *fp = fopen("movies.txt", "w");
    if(fp == NULL) {
        printf("\nError saving movies!");
        return;
    }

    for(int i = 0; i < num_movies; i++) {
        fprintf(fp, "%s,%d,%s,%d,%d,%s,%s,%d",
               movies[i].name,
               movies[i].code,
               movies[i].description,
               movies[i].premium_price,
               movies[i].regular_price,
               movies[i].date,
               movies[i].time,
               movies[i].hall_number);
        
        
        for (int j = 0; j < MAX_PREMIUM_SEATS; j++) {
            fprintf(fp, ",%d", movies[i].premium_seats[j]);
        }
        
        
        for (int j = 0; j < MAX_REGULAR_SEATS; j++) {
            fprintf(fp, ",%d", movies[i].regular_seats[j]);
        }
        
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void load_movies() {
    FILE *fp = fopen("movies.txt", "r");
    if(fp == NULL) return;

    while(!feof(fp) && num_movies < MAX_MOVIES) {
        Movie m = {0};
        if(fscanf(fp, "%49[^,],%d,%99[^,],%d,%d,%19[^,],%19[^,],%d",
               m.name, &m.code, m.description, &m.premium_price,
               &m.regular_price, m.date, m.time, &m.hall_number) != 8) {
            continue;
        }
        
        
        for (int i = 0; i < MAX_PREMIUM_SEATS; i++) {
            int seat_status;
            if (fscanf(fp, ",%d", &seat_status) != 1) {
                seat_status = 0;
            }
            m.premium_seats[i] = seat_status;
        }
        
        
        for (int i = 0; i < MAX_REGULAR_SEATS; i++) {
            int seat_status;
            if (fscanf(fp, ",%d", &seat_status) != 1) {
                seat_status = 0;
            }
            m.regular_seats[i] = seat_status;
        }
        
        movies[num_movies++] = m;
    }
    fclose(fp);
}

void save_bookings() {
    FILE *fp = fopen("bookings.txt", "a"); 
    if (fp == NULL) {
        printf("\nError saving bookings!");
        return;
    }

    
    Booking *booking = &bookings[num_bookings - 1];
    fprintf(fp, "%d,%s,%llu,%d,%s,%d,%d,%d,",
            booking->booking_id,
            booking->customer_name,
            booking->mobile_number,
            booking->movie_code,
            booking->movie_name,
            booking->hall_number,
            booking->premium_tickets,
            booking->regular_tickets);

    
    for (int j = 0; j < booking->premium_tickets; j++) {
        fprintf(fp, "%s", booking->premium_seats[j]);
        if (j < booking->premium_tickets - 1) fprintf(fp, ";");
    }

    fprintf(fp, ",");

    
    for (int j = 0; j < booking->regular_tickets; j++) {
        fprintf(fp, "%s", booking->regular_seats[j]);
        if (j < booking->regular_tickets - 1) fprintf(fp, ";");
    }

    fprintf(fp, ",%.2f,%.2f,%.2f,%s,%s,%s\n",
            booking->total_price,
            booking->discount,
            booking->tax,
            booking->payment_method,
            booking->purchase_date,
            booking->purchase_time);

    fclose(fp);
}

void load_bookings() {
    FILE *fp = fopen("bookings.txt", "r");
    if (fp == NULL) {
        printf("\nNo previous bookings found.\n");
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), fp) && num_bookings < MAX_BOOKINGS) {
        Booking b = {0};
        char *token = strtok(line, ",");
        if (!token) continue;
        b.booking_id = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(b.customer_name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.mobile_number = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");  
        if (!token) continue;
        b.movie_code = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(b.movie_name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.hall_number = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.premium_tickets = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.regular_tickets = atoi(token);

        
        token = strtok(NULL, ",");
        if (token && b.premium_tickets > 0) {
            char *seat_token = strtok(token, ";");
            for (int i = 0; i < b.premium_tickets && seat_token; i++) {
                strcpy(b.premium_seats[i], seat_token);
                seat_token = strtok(NULL, ";");
            }
        }

        
        token = strtok(NULL, ",");
        if (token && b.regular_tickets > 0) {
            char *seat_token = strtok(token, ";");
            for (int i = 0; i < b.regular_tickets && seat_token; i++) {
                strcpy(b.regular_seats[i], seat_token);
                seat_token = strtok(NULL, ";");
            }
        }

        token = strtok(NULL, ",");
        if (!token) continue;
        b.total_price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.discount = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        b.tax = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(b.payment_method, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(b.purchase_date, token);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strcpy(b.purchase_time, token);

        bookings[num_bookings++] = b;
        if (b.booking_id > last_booking_id) {
            last_booking_id = b.booking_id;
        }
    }
    fclose(fp);
}