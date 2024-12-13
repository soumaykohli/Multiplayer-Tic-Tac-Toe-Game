#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BOARD_SIZE 9

// Function to handle client communication and manage game state
void handleClient(int client1, int client2) {
    char board[BOARD_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int turn = 0;

    while (1) {
        int currentPlayer = (turn % 2 == 0) ? client1 : client2;
        int otherPlayer = (turn % 2 == 0) ? client2 : client1;

        send(currentPlayer, board, sizeof(board), 0);
        char move[2];
        read(currentPlayer, move, 2);  // Read the move

        int moveIndex = move[0] - '1';  // Convert to index
        if (board[moveIndex] == 'X' || board[moveIndex] == 'O') {
            send(currentPlayer, "Invalid move!", strlen("Invalid move!"), 0);
        } else {
            board[moveIndex] = (turn % 2 == 0) ? 'X' : 'O';
            send(otherPlayer, board, sizeof(board), 0);
            turn++;
        }
    }
}

int main() {
    int server_fd, client1, client2;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Binding to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for clients
    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for players...\n");

    if ((client1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Player 1 connected!\n");

    if ((client2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Player 2 connected!\n");

    handleClient(client1, client2);

    close(client1);
    close(client2);
    close(server_fd);
    return 0;
}
