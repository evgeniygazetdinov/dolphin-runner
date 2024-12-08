#include <switch.h>
#include "string"
#include <cstdio>
#include <thread>
#include <sys/stat.h>


const int KEY_X = 4;
const int KEY_Y = 28;
const int KEY_A = 5;
const int KEY_B = 27;
const int KEY_PLUS = 1024;
const int KEY_MINUS = 2048;

void log_to_file(const char* message) {
    FILE* log_file = fopen("sdmc:/switch/dolphin-emu/dolphin-launcher.log", "a");
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[26];
        ctime_r(&now, timestamp);
        timestamp[24] = '\0'; // Убираем перенос строки
        fprintf(log_file, "[%s] %s\n", timestamp, message);
        fclose(log_file);
    }
}



// Function to execute a command on Switch with arguments
bool executeCommand(const char* path, const char* args) {

    // Корректно завершаем текущие сервисы
    romfsExit();
    consoleExit(NULL);
    NcmProgramLocation loc = {0};
    Result rc;
    char buff[200];
    snprintf(buff, sizeof(buff), "%s %s",path, args);
    std::string runnedProcess = buff;
    // Запускаем процесс
    u64 pid = ldrPmCreateProcess(2,2,0,0);
    rc = pmshellLaunchProgram(0, reinterpret_cast<const NcmProgramLocation *>(std::addressof(runnedProcess)), &pid);
    if (R_FAILED(rc)) return rc;

    return true; // Этот return может не выполниться
}

bool mainLoop() {
    printf("\n\n-------- Main Menu --------\n");
    printf("Press B to run dolpin emulator\n");
    printf("Press - to exit\n");
    log_to_file("Main menu started");

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
    bool needToRun = false;
    char* args;
    char* dolphin_cmd;
    while (appletMainLoop()) {
        // Сканируем ввод
        
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & KEY_MINUS) {
            printf("Exiting...\n");
            log_to_file("Exiting...");
            return false;
        }

        if (kDown & KEY_B) {
            printf("\nStarting Dolphin launch process...\n");
            log_to_file("Starting Dolphin launch process...");
            needToRun = true;
        }
        if (needToRun){
            // Записываем последнее сообщение в лог
            const char* success_msg = "Attempting to launch Dolphin...";

            log_to_file(success_msg);
            const char* path = "sdmc:/switch/dolphin-emu-nogui.nro";
            const char* args = "-e sdmc:/switch/dolphin-emu/roms/DeadSpace.iso;";
            // Запускаем команду - после этого наша программа может быть уже не выполнена
            printf("%s %s", path,args);
            executeCommand(path, args);
        }
                consoleUpdate(NULL);
        svcSleepThread(100000000ULL);
    }        

    return true;
}


int main(int argc, char* argv[]) {
    // Инициализация консоли

    consoleInit(NULL);
    log_to_file("Program started");
    mainLoop();
    consoleExit(NULL);
    return 0;
}