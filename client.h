#include <thread>
#include <vector>
#include <atomic>
class Client {
    public:
        Client(string ip, int port);
        ~Client();
        void start();
        string uploadfile(DefaultIO* dio, string file_path);
        void downloadfile(DefaultIO* dio, string file_path, string content);
        bool validChoice(string m);
        // void* downloadWrapper(void* args);
    private:
        DefaultIO* dio;
        int port;
        string ip;
};
