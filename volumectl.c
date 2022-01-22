#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_volume();
void send_notification();

// TODO: Replace all system() amixer call with functions from alsa lib

int main(int argc, char *argv[])
{
    // 1st argument should alwatys be "up" or "down" depending on whether you want to increase or decrease the volume
    const char* ctrl = argv[1];

    if(strcmp(ctrl, "up") == 0) 
    {
        // Increase the volume
        system("amixer set 'Master' 5%+ > /dev/null");
        send_notification();
        return 0;
    }
    else if(strcmp(ctrl, "down") == 0)
    {
        // Decrease the volume
        system("amixer set 'Master' 5%- > /dev/null");
        send_notification();
        return 0;
    }
    else
    {
        printf("Invalid argument");
        return 1;
    }
}

int get_volume()
{
    FILE* vol_file = popen("amixer -D pulse get Master | grep '%' | head -n 1 | cut -d '[' -f 2 | cut -d '%' -f 1", "r");
    int volume;
    fscanf(vol_file, "%d", &volume);
    pclose(vol_file);
    return volume;
}

void send_notification()
{
    int true_vol = get_volume();
    
    // `-` = 5 volume unit
    int volume = true_vol / 5;
    if(volume >= 20) volume = 20;
    if(volume <= 0) volume = 0;

    const char bar_fmt[] = "seq -s \"─\" %d | sed 's/[0-9]//g'";
    char* bar_cmd = (char*) malloc(sizeof(char) * 36);
    sprintf(bar_cmd, bar_fmt, volume);

    FILE* bar_file = popen(bar_cmd, "r");
    free(bar_cmd);
    
    char bar[21] = { 0 };
    fscanf(bar_file, "%s", bar);
    pclose(bar_file);

    char* notify_cmd = (char*) malloc(sizeof(char) * 100); // [54 + 21] = { 0 };
    sprintf(notify_cmd,"dunstify -r 2593 \"🔈 %d %s\"", true_vol, bar);

    system(notify_cmd);

    free(notify_cmd);
}
