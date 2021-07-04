#pragma once

class Concentrator {
    private:
        static const int Sieve_A_Valve ; //Defined Pin as Variable
        static const int Sieve_B_Valve ; //Defined Pin as Variable
        static const int PreCharge_Valve ; //Defined Pin as Variable
        static const int Fan ; //Defined Pin as Variable

        static const int Relay_Test_Delay ; //delay variable creation
        static const int Startup_Purge_Delay ; //delay variable creation
        static const int Production_Delay ; //delay variable creation
        static const int Flush_Delay ; //delay variable creation
        static const int PreCharge_Delay ; //delay variable creation

    public:
    /*
        Concentrator(const int& A_valve,
                     const int& B_valve, 
                     const int& P_valve, 
                     const int& fan,
                     const int& relay_test_delay, 
                     const int& startup_purge_delay, 
                     const int& production_delay, 
                     const int& flush_delay, 
                     const int& precharge_delay);
    */
        static void set_relay_pins(const int& A_valve, const int& B_valve, const int& P_valve, const int& fan);
        static void set_pinmode_for_program(void);
        static void set_delay_timing(const int& relay_test_delay, 
                                const int& startup_purge_delay, 
                                const int& production_delay, 
                                const int& flush_delay, 
                                const int& precharge_delay);
        static void valve_relay_test_sequence(void);
        static void startup_purge(void);
        static void fan_control(void);
        static void run(void);
        static void ConctrRun_Task(void *parameters);
        static void begin();

    private:
        static void cycle_1(void);
        static void cycle_2(void);
        static void cycle_3(void);
        static void cycle_4(void);
        static void cycle_5(void);
        static void cycle_6(void);

};
