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
        void getTitle(DefaultIO* io);
        void choiceOne(DefaultIO* io, string user_input);
        void choiceTwo(DefaultIO* io, string user_input);
        bool validChoice(string m);
        // void* downloadWrapper(void* args);
    private:
        thread* download_thread;
        atomic<bool> running;
        DefaultIO* dio;
        int port;
        string ip;
};
