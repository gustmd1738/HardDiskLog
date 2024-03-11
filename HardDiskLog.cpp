#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum checkMode
{
    LogicalDisk = 0,
    PhysicalDisk,
    Network,
    Memory,
    CPU,
    Processes,
    Threads
};

vector<string> split(const string& s, const vector<string>& delims) {
    vector<string> result;
    size_t start = 0;
    size_t pos;
    while (start < s.length()) {
        pos = string::npos;
        for (const auto& delim : delims) {
            size_t found = s.find(delim, start);
            if (found != string::npos && (pos == string::npos || found < pos)) {
                pos = found;
            }
        }
        if (pos == string::npos) {
            result.push_back(s.substr(start));
            break;
        }
        result.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return result;
}

void consoleOutput(const vector<string>& result, checkMode mode) {
    int hardNum;
    switch (mode) {
    case LogicalDisk:
    case PhysicalDisk:
        hardNum = (result.size() - 5) / 2;
        if (mode == LogicalDisk) {
            cout << "---------------------------------------------------" << endl;
            cout << result[hardNum + 2] << endl;
        }
        for (int i = 2; i + hardNum + 1 < result.size() - 2; i++) {
            cout << result[i] << " : " << result[i + hardNum + 1] << endl;
        }
        break;

    case Network:
    case Memory:
    case CPU:
    case Processes:
    case Threads:
        hardNum = (result.size() - 6) / 2;
        for (int i = 2; i + hardNum + 1 < result.size() - 3; i++) {
            cout << result[i] << " : " << result[i + hardNum + 1] << endl;
        }
        if (mode == Threads) {
            cout << "---------------------------------------------------" << endl;
        }
        break;
    }
}

void writeResults(const string& result, checkMode mode) {
    vector<string> lines = split(result, { ",", "\n" });

    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    char dateBuffer[80];
    strftime(dateBuffer, 80, "%Y-%m-%d", &localTime);
    string fileName = "Disk Time_" + string(dateBuffer) + ".txt";

    ofstream outputFile(fileName, ios::app);

    if (outputFile.is_open()) {
        int hardNum;
        switch (mode) {
        case LogicalDisk:
        case PhysicalDisk:
            hardNum = (lines.size() - 5) / 2;

            if (mode == LogicalDisk) {
                outputFile << "---------------------------------------------------" << endl;
                outputFile << lines[hardNum + 2] << endl;
            }

            for (int i = 2; i + hardNum + 1 < lines.size() - 2; i++) {
                outputFile << lines[i] << " : " << lines[i + hardNum + 1] << endl;
            }
            break;

        case Network:
        case Memory:
        case CPU:
        case Processes:
        case Threads:
            hardNum = (lines.size() - 6) / 2;
            for (int i = 2; i + hardNum + 1 < lines.size() - 3; i++) {
                outputFile << lines[i] << " : " << lines[i + hardNum + 1] << endl;
            }

            if (mode == Threads) {
                outputFile << "---------------------------------------------------" << endl;
            }
            break;
        }

        outputFile.close();
        consoleOutput(lines, mode);
    }
    else {
        cout << "Unable to open file: " << fileName << "\t mode : " << mode << endl;
    }
}

string checkPerformance(const string& command) {
    stringstream resultStream;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        resultStream << "Error: Unable to open pipe";
    }
    else {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                resultStream << buffer;
            }
        }
        _pclose(pipe);
    }

    return resultStream.str();
}

int main() {

    while (true) {
#ifdef _DEBUG
        cout << "Debug 모드에서 실행 중입니다." << endl;
#endif
        string result;

        int sequence = 0;

        while (sequence < 7)
        {
            switch (sequence)
            {
            case 0:
                // 디스크 부하율 확인
                result = checkPerformance("typeperf \"\\LogicalDisk(*)\\% Disk Time\" -sc 1");
                writeResults(result, LogicalDisk);
                break;
            case 1:
                // Disk IO 확인
                result = checkPerformance("typeperf \"\\PhysicalDisk(*)\\% Disk Time\" -sc 1");
                writeResults(result, PhysicalDisk);
                break;
            case 2:
                // Network 확인
                result = checkPerformance("typeperf \"\\Network Interface(*)\\Bytes Total/sec\" -sc 1");
                writeResults(result, Network);
                break;
            case 3:
                // 메모리 확인
                result = checkPerformance("typeperf \"\\Memory\\% Committed Bytes In Use\" -sc 1");
                writeResults(result, Memory);
                break;
            case 4:
                // CPU 확인
                result = checkPerformance("typeperf \"\\Processor(_Total)\\% Processor Time\" -sc 1");
                writeResults(result, CPU);
                break;
            case 5:
                // Processes 확인
                result = checkPerformance("typeperf \"\\System\\Processes\" -sc 1");
                writeResults(result, Processes);
                break;
            case 6:
                // Threads 확인
                result = checkPerformance("typeperf \"\\System\\Threads\" -sc 1");
                writeResults(result, Threads);
                break;
            }

            sequence++;

            this_thread::sleep_for(chrono::seconds(1));

        }
        
        // 다음 사이클 대기 시간
#ifdef _DEBUG
        this_thread::sleep_for(chrono::seconds(10));
#else
        this_thread::sleep_for(chrono::minutes(10));
#endif
    }

    return 0;
}