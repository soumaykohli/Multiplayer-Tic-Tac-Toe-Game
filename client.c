#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

// Function to display the board
void displayBoard(char board[]) {
    for (int i = 0; i < 9; i += 3) {
        printf(" %c | %c | %c\n", board[i], board[i + 1], board[i + 2]);
        if (i < 6) printf("---|---|---\n");
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char board[9];
    char move[2];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    while (1) {
        read(sock, board, sizeof(board));
        displayBoard(board);
        printf("Enter your move (1-9): ");
        scanf("%s", move);
        send(sock, move, strlen(move), 0);
    }

    close(sock);
    return 0;
}
