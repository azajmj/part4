#include <thread>
#include <vector>
#include <atomic>
class Server {
    public:
        Server(int port);
        ~Server();
        void start();
        void stop();
        void handleClient(int client_socket, CLI &cli);
    private:
        vector<thread> threads;
        atomic<bool> running;
        int socket;
}
