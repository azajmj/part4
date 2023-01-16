#include <thread>
#include <vector>
#include <atomic>
#include "DefaultIO.h"
class Server {
    public:
        Server(int port);
        ~Server();
        void start();
        void handleClient(DefaultIO* dio, int client_sock);
    private:
        vector<thread> threads;
        int port;
};
