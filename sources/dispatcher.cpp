#include "dispatcher.hpp"

int main( int argc, char** argv ){

    //     filename = "experiment_";
    //     filename = filename + argv[1];



    //     std::ofstream myfile;
    //     myfile.open (filename, std::ofstream::out | std::ofstream::app);
    //     myfile << "Experiment" << std::endl;
    //     myfile.close();

    /*
    * Connect to SmartSpace
    */
    std::cout << "Trying to connect to SmartSpace with\n";
    std::cout << "Name: " << KP_SS_NAME << "\n";
    std::cout << "IP: " << KP_SS_ADDRESS << "\n";
    std::cout << "Port: " << KP_SS_PORT << "\n";

    if( 0 != connect_to_smartspace() ){
        std::cout << "Error in ss initialization.\n";

        sslog_shutdown();
        return -1;
    }

    std::cout << "Connection is successful.\n" << "\n";

    std::cout << "Trying to start subscription thread...\n";
    boost::thread t0{sbcr_thread};

    std::cout << "Subscription thread was started\n\n";

    std::cout << "Trying to start alarm thread...\n";
    boost::thread t1{alarm_thread};

    std::cout << "Alarm thread was started\n\n";

    std::cout << "Volunteer dispatcher is started. To shutdown it press CTRL+C\n";

    global_stop_signal_recieved = false;
    signal(SIGINT, ctrl_c_handler);
    while(!global_stop_signal_recieved) { getchar(); }

    t0.join();
    t1.join();
    return 0;
}
void ctrl_c_handler(int dummy) {
    global_stop_signal_recieved = true;
    std::cout << "Stop signal\n";
    exit(0);
}
