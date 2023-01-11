#include <thread>
#include <vector>
#include <atomic>
class Client {
    public:
        Client(string ip, int port);
        ~Client();
        void start();
        void uploadfile(DefaultIO* dio, string file_path);
        void downloadfile(DefaultIO* dio, string file_path);
    private:
        thread* download_thread;
        atomic<bool> running;
        DefaultIO* dio;
        int port;
        string ip;
};
