#include "cnt_high_voltage_gbs.h"



cnt_high_voltage_gbs::cnt_high_voltage_gbs(/* args */)
{
}

cnt_high_voltage_gbs::~cnt_high_voltage_gbs()
{
}

void  cnt_high_voltage_gbs::disconnect()
{

}
wgm_feedbacks::enum_sub_sys_feedback cnt_high_voltage_gbs::connect()
{
return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
}
void cnt_high_voltage_gbs::waitForResponse()
{

}
void cnt_high_voltage_gbs::start()
{

}
void cnt_high_voltage_gbs::stop()
{

}
void cnt_high_voltage_gbs::tune_param(std::pair<std::string, int> new_param_val)
{

} // tune param

bool cnt_high_voltage_gbs::getStatus()
{
return hvReady;
}

void cnt_high_voltage_gbs::pulse()
{
    std::cout<<"sending pulse.."<<std::endl;
}

void cnt_high_voltage_gbs::modulate()
{
    std::cout<<"sending pulse.."<<std::endl;
}

 double cnt_high_voltage_gbs::get_input_current()
 {
    return 0;
 }
 double cnt_high_voltage_gbs::get_input_frequency()
 {
    return 0;

 }
 double cnt_high_voltage_gbs::get_input_voltage()
 {
    return 0;

 }
 double cnt_high_voltage_gbs::get_output_voltage()
 {
    return 0;

 }
 double cnt_high_voltage_gbs::get_output_frequency()
 {
    return 0;

 }
 double cnt_high_voltage_gbs::get_output_current()
 {
    return 0;

 }