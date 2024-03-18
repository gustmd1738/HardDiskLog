#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

std::string fileFath = "C:\\Users\\USER\\Desktop\\PC1\\Disk Log_2024-03-16 - 복사본.txt"; // 파일 경로를 여기에 입력하세요
std::string desktopPath = "C:\\Users\\USER\\Desktop\\"; // 저장 경로
int main()
{
    std::string fileName = "";
    for (int i = fileFath.size()-1; i >= 0; i--)
    {
        if (fileFath[i] == '\\')
        {
            for (int j = i + 1; j < fileFath.size(); j++)
            {
                if (fileFath[j] == '.')
                    break;

                fileName += fileFath[j];
            }
            break;
        }
    }


    std::string graphFileName = fileName + " graph" + ".csv";

    if (std::filesystem::exists(desktopPath + graphFileName))
    {
        std::cout << "파일이 이미 존재합니다." << std::endl;
        return 0;
    }

    std::ifstream file(fileFath);

    if (file.is_open())
    {
        std::ofstream outputFile(desktopPath + graphFileName);
        if (outputFile.is_open())
        {
            std::string line;
            //csv파일에 열쓰기
            bool writingColumn = false;
            bool writingFirst = true;
            while (getline(file, line))
            {
                if (line == "---------------------------------------------------")
                {
                    if (!writingColumn)
                    {
                        writingColumn = true;
                        continue;
                    }
                    else
                    {
                        writingColumn = false;
                        break;
                    }
                }

                if (writingColumn && writingFirst)
                {
                    outputFile << "Time,";
                    writingFirst = false;
                    continue;
                }
                else if (writingColumn && !writingFirst)
                {
                    bool start = false;
                    std::string temp = "";
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (i == 0)
                        {
                            if (line[i] == 'n')
                            {
                                outputFile << "GPU";
                                break;
                            }
                        }

                        if (line[i] == ':' && line[i - 1] == ' ' && line[i + 1] == ' ')
                        {
                            break;
                        }

                        temp += line[i];
                    }
                    outputFile << temp;
                    outputFile << " , ";
                }


            }

            line.clear();
            outputFile << "\n";
        
            bool writingRow = false;
            int LogLine = 0;

            file.clear();
            file.seekg(0, std::ios::beg);

            while (getline(file, line))
            {
                if (line == "---------------------------------------------------")
                {
                    if (!writingRow)
                    {
                        writingRow = true;
                        LogLine = 0;
                        continue;
                    }
                    else
                    {
                        writingRow = false;
                        outputFile << "\n";
                        continue;
                    }
                }

                if (writingRow)
                {
                    bool start = false;
                    std::string temp = "";
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (LogLine == 0)
                        {
                            if (i == 0)
                                continue;

                            if (i == line.size() - 1)
                            {
                                break;
                            }

                            temp += line[i];
                            continue;
                        }

                        if (line[i] == ':' && line[i - 1] == ' ' && line[i + 1] == ' ')
                        {
                            start = true;
                            continue;
                        }

                        if(start)
                            temp += line[i];
                    }

                    if (LogLine != 0)
                    {
                        const char charsToRemove[] = { ' ', '"' };
                        for (char c : charsToRemove) {
                            temp.erase(std::remove(temp.begin(), temp.end(), c), temp.end());
                        }
                    }

                    outputFile << temp;
                    outputFile << ',';
                
                }

                LogLine++;
            }
        
            outputFile.close();
            file.close();
            std::cout << desktopPath << " 경로에 " << graphFileName << " 파일을 생성하였습니다." << std::endl;

        }
        else
        {
            std::cout << graphFileName << " 파일을 열 수 없습니다." << std::endl;
        }
        
        
    }
    else
    {
        std::cout << "해당경로의 파일을 열 수 없습니다." << std::endl;
    }
    
    return 0;
}