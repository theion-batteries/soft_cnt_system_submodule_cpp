#include "cnt_hvac_gbs.h"



cnt_hvac_gbs::cnt_hvac_gbs(const std::string &ip, const uint16_t port)
{
        std::cout << "creating CNT high voltage client" << std::endl;
        
        _hv_server.ip=ip;
        _hv_server.port=port;
}

cnt_hvac_gbs::~cnt_hvac_gbs()
{
}



wgm_feedbacks::enum_sub_sys_feedback cnt_hvac_gbs::disconnect()
{
    if ( !_client->is_connected() || _client->close() ) {
    hvReady = false;
    return sub_success;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_hvac_gbs::connect()
{
    std::cout << "connecting controller to hv server" << std::endl;
    auto hv_server_addr = sockpp::tcp_connector::addr_t{_hv_server.ip,_hv_server.port};

     _client =std::make_unique<sockpp::tcp_connector>(hv_server_addr);

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (! _client->is_connected()) {
        std::cerr << "Error connecting to hv server at "
            << sockpp::inet_address(_hv_server.ip, _hv_server.port)
            << " -> " <<  _client->last_error_str();
        hvReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " <<  _client->address() << std::endl;
    std::cout << "Created a connection to " <<  _client->peer_address() << std::endl;
    // Set a timeout for the responses
    if (! _client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            <<  _client->last_error_str() << std::endl;
        hvReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    hvReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

std::string cnt_hvac_gbs::sendDirectCmd(std::string cmd) {
 if (_client.get() == nullptr) return "not connected";
     std::cout << "sending hvac command " << cmd << std::endl;
    cmd = cmd + "\r\n";
    if (_client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << _client->last_error_str() << std::endl;
    }
    return waitForResponse();
}

auto cnt_hvac_gbs::process_cmd(std::string cmd,std::string args="") {
    auto command=hv_cmds.find(cmd);
    if (command != hv_cmds.end()) {
        std::cout << "sending command: " << command->second << "\n";
        auto reply = sendDirectCmd(command->second+args);
        return reply;
    }
    return std::string("");
}

std::string cnt_hvac_gbs::waitForResponse()
{
 std::cout << "awaiting server response" << std::endl;
    auto start = std::chrono::steady_clock::now();
    while (_client->is_connected())
    {

        char Strholder[5012];
        
            ssize_t n = _client->read_n(&Strholder, sizeof(Strholder));
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            incoming_data = Strholder;
            incoming_data.resize(n);
            std::cout << "server replied : " << incoming_data << std::endl;
            break;
        }
        else
        {
            std::cout << "no server response, retry " << n << std::endl;
            incoming_data = "NA";
            long long timeout = 10;
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();
            if(duration >= timeout)
            {
            std::cout << "no response within a timeout of "<<duration<< " seconds, " <<"aborting.."<< std::endl;
            break;
            } 
            continue;
        }

    }
    return incoming_data;
}

enum_sub_sys_feedback cnt_hvac_gbs::start()
{
    std::cout << "start high voltage AC" << std::endl;
    auto response = process_cmd("START");
    if (response=="ok") return sub_success;
    return sub_error;
}

enum_sub_sys_feedback cnt_hvac_gbs::stop()
{
    std::cout << "stop high voltage AC" << std::endl;
    auto response = process_cmd("STOP");
    if (response=="ok") return sub_success;
    return sub_error;
}


enum_sub_sys_feedback cnt_hvac_gbs::tune_param(std::pair<std::string, int> new_param_val)
{
  return sub_error;
} // tune param

bool cnt_hvac_gbs::getStatus()
{
return hvReady;
}

enum_sub_sys_feedback cnt_hvac_gbs::pulse()
{
    std::cout<<"sending pulse.."<<std::endl;
    return sub_error;
}

enum_sub_sys_feedback cnt_hvac_gbs::modulate()
{
    std::cout<<"sending pulse.."<<std::endl;
    return sub_error;
}

double cnt_hvac_gbs::get_input_current()
{
return 0;
}

double cnt_hvac_gbs::get_input_frequency()
{
 return 0;
}

double cnt_hvac_gbs::get_input_voltage()
{
    return 0;
}



double cnt_hvac_gbs::get_output_voltage()
{    
    auto voltage = process_cmd("OUTPUT_VOLT");
    if(voltage=="") return 0;
    return std::stod(voltage);
}
 
double cnt_hvac_gbs::get_output_frequency()
{
    auto freq = process_cmd("OUTPUT_FREQ");
    if(freq=="") return 0;
    return std::stod(freq);
}
 
double cnt_hvac_gbs::get_output_current()
{
    auto current = process_cmd("OUTPUT_CURRENT");
    if(current=="") return 0;
    return std::stod(current);
}

 enum_sub_sys_feedback cnt_hvac_gbs::set_output_voltage(const double voltage) {
    
    auto response = process_cmd("SET_OUTPUT_VOLT","="+std::to_string(voltage));
    if (response=="ok") return sub_success;
    return sub_error;   
 }


double cnt_hvac_gbs::get_output_resistivity() {
    auto voltage = process_cmd("OUTPUT_VOLT");
    if(voltage=="") return 0;

    auto current = process_cmd("OUTPUT_CURRENT");
    if(current=="") return 0;
    return (std::stod(voltage)/std::stod(current));
}

enum_sub_sys_feedback cnt_hvac_gbs::set_output_frequency(const double frequency) {
   auto response = process_cmd("SET_OUTPUT_FREQ","="+std::to_string(frequency));
    if (response=="ok") return sub_success;
    return sub_error;   
}
