#include <iostream>
using namespace std;

class Clock
{
public:
    void SetTime(int h, int m, int s);
    void ShowTime(void);
    
private:
    int hour;
    int minute;
    int second;  
};

void Clock::SetTime(int h, int m, int s)
{
    hour = h;
    minute = m;
    second = s;
}

void Clock::ShowTime(void)
{
    cout << "Time Now: " << hour << ":" << minute << ":" << second << endl;
}

int main(int argc, char *argv[])
{
    Clock clock;
    
    clock.SetTime(16, 29, 30);
    clock.ShowTime();
    
    return 0;
}
