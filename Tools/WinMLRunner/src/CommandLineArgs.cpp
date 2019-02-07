#include <Windows.h>
#include <string>
#include <iostream>
#include "CommandLineArgs.h"
#include "Filehelper.h"

using namespace Windows::AI::MachineLearning;
bool g_loadDllFromLocal = false; //global flag to check if user wants to load Windows.AI.MachineLearning.dll from local directory.
std::wstring g_loadWinMLDllPath = L""; //If user wants to load Windows.AI.MachineLearning.dll from specific local directory, this will hold the DLL path.
void CommandLineArgs::PrintUsage() {
    std::cout << "WinML Runner" << std::endl;
    std::cout << " ---------------------------------------------------------------" << std::endl;
    std::cout << "WinmlRunner.exe <-model | -folder> <fully qualified path> [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "options: " << std::endl;
    std::cout << "  -CPU : run model on default CPU" << std::endl;
    std::cout << "  -GPU : run model on default GPU" << std::endl;
    std::cout << "  -GPUHighPerformance : run model on GPU with highest performance" << std::endl;
    std::cout << "  -GPUMinPower : run model on GPU with the least power" << std::endl;
    std::cout << "  -CreateDeviceOnClient : create the device on the client and pass it to WinML" << std::endl;
    std::cout << "  -CreateDeviceInWinML : create the device inside WinML" << std::endl;
    std::cout << "  -CPUBoundInput : bind the input to the CPU" << std::endl;
    std::cout << "  -GPUBoundInput : bind the input to the GPU" << std::endl;
    std::cout << "  -RGB : load the input as an RGB image" << std::endl;
    std::cout << "  -BGR : load the input as a BGR image" << std::endl;
    std::cout << "  -Tensor : load the input as a tensor" << std::endl;
    std::cout << "  -Perf : capture timing measurements" << std::endl;
    std::cout << "  -Iterations : # times perf measurements will be run/averaged" << std::endl;
    std::cout << "  -Input <fully qualified path>: binds image or CSV to model" << std::endl;
    std::cout << "  -PerfOutput optional:<fully qualified path>: csv file to write the perf results to" << std::endl;
    std::cout << "  -IgnoreFirstRun : ignore the first run in the perf results when calculating the average" << std::endl;
    std::cout << "  -SavePerIterationPerf : save per iteration performance results to csv file" << std::endl;
    std::cout << "  -SaveTensorData <saveMode>: save first iteration or all iteration output tensor results to csv file [First, All]" << std::endl;
    std::cout << "  -Debug: print trace logs" << std::endl;
    std::cout << "  -Terse: Terse Mode (suppresses repetitive console output)" << std::endl;
    std::cout << "  -AutoScale <interpolationMode>: Enable image autoscaling and set the interpolation mode [Nearest, Linear, Cubic, Fant]" << std::endl;
    std::cout << "  -LoadWinMLFromLocal optional:<fully qualified path> Loads Windows.Ai.MachineLearning.dll from optionally defined folder path. If path isn't specified then the DLL will be loaded from the same directory as WinMLRunner.exe" << std::endl;
}

CommandLineArgs::CommandLineArgs(const std::vector<std::wstring>& args)
{
    for (UINT i = 0; i < args.size(); i++)
    {
        if ((_wcsicmp(args[i].c_str(), L"-CPU") == 0))
        {
            m_useCPU = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-GPU") == 0))
        {
            m_useGPU = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-GPUHighPerformance") == 0))
        {
            m_useGPUHighPerformance = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-GPUMinPower") == 0))
        {
            m_useGPUMinPower = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-CreateDeviceOnClient") == 0))
        {
            m_createDeviceOnClient = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-CreateDeviceInWinML") == 0))
        {
            m_createDeviceInWinML = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Iterations") == 0) && (i + 1 < args.size()))
        {
            m_numIterations = static_cast<UINT>(_wtoi(args[++i].c_str()));
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Model") == 0) && (i + 1 < args.size()))
        {
            m_modelPath = args[++i];
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Folder") == 0) && (i + 1 < args.size()))
        {
            m_modelFolderPath = args[++i];
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Input") == 0))
        {
            m_inputData = args[++i];
        }
        else if ((_wcsicmp(args[i].c_str(), L"-PerfOutput") == 0))
        {
            if (i + 1 < args.size() && args[i+1][0] != L'-')
            {
                m_perfOutputPath = args[++i];
            }
            m_perfOutput = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-RGB") == 0))
        {
            m_useRGB = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-BGR") == 0))
        {
            m_useBGR = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Tensor") == 0))
        {
            m_useTensor = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-CPUBoundInput") == 0))
        {
            m_useCPUBoundInput = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-GPUBoundInput") == 0))
        {
            m_useGPUBoundInput = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-IgnoreFirstRun") == 0))
        {
            m_ignoreFirstRun = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Perf") == 0))
        {
            m_perfCapture = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Debug") == 0))
        {
            m_debug = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-SavePerIterationPerf") == 0))
        {
            m_perIterCapture = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-Terse") == 0))
        {
            m_terseOutput = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"-AutoScale") == 0) && (i + 1 < args.size()))
        {
            m_autoScale = true;
            if (_wcsicmp(args[++i].c_str(), L"Nearest") == 0)
            {
                m_autoScaleInterpMode = BitmapInterpolationMode::NearestNeighbor;
            }
            else if (_wcsicmp(args[i].c_str(), L"Linear") == 0)
            {
                m_autoScaleInterpMode = BitmapInterpolationMode::Linear;
            }
            else if (_wcsicmp(args[i].c_str(), L"Cubic") == 0)
            {
                m_autoScaleInterpMode = BitmapInterpolationMode::Cubic;
            }
            else if (_wcsicmp(args[i].c_str(), L"Fant") == 0)
            {
                m_autoScaleInterpMode = BitmapInterpolationMode::Fant;
            }
            else
            {
                std::cout << "Unknown AutoScale Interpolation Mode!" << std::endl;
                PrintUsage();
                return;
            }
        }
        else if ((_wcsicmp(args[i].c_str(), L"-SaveTensorData") == 0) && (i + 1 < args.size()))
        {
            m_saveTensor = true;
            if (_wcsicmp(args[++i].c_str(), L"First") == 0)
            {
                m_saveTensorMode = "First";
            }
            else if (_wcsicmp(args[i].c_str(), L"All") == 0)
            {
                m_saveTensorMode = "All";
            }
            else
            {
                std::cout << "Unknown Mode!" << std::endl;
                PrintUsage();
                return;
            }
        }
        else if ((_wcsicmp(args[i].c_str(), L"-LoadWinMLFromLocal") == 0))
        {
            std::cout << "Loading Windows.AI.MachineLearning.dll from local directory specified. Validating if this can be done.." << std::endl;
            if (i + 1 < args.size() && args[i+1][0] != L'-')
            {
                std::cout << "Folder path containing Windows.Ai.MachineLearning.dll was specified, attempting to load from path.. " << std::endl;
                HMODULE library = LoadLibraryW((args[i + 1] + L"Windows.AI.MachineLearning.dll").c_str());
                if (!library)
                {
                    throw hresult_invalid_argument(L"ERROR: Loading Windows.Ai.MachineLearning.dll from local failed! Check if it is contained in the path that was specified.");
                }
                else
                {
                    std::wcout << "Successfully validated: " << args[i + 1] + L"Windows.AI.MachineLearning.dll" << std::endl;
                }
                g_loadWinMLDllPath = args[++i];
            }
            else
            {
                std::cout << "Windows.Ai.MachineLearning.dll path wasn't specified, attempting to load from same directory as WinMLRunner.exe" << std::endl;
                HMODULE library = LoadLibraryW((FileHelper::GetModulePath() + L"Windows.AI.MachineLearning.dll").c_str());
                if (!library)
                {
                    throw hresult_invalid_argument(L"ERROR: Loading Windows.Ai.MachineLearning.dll from local failed! Check if it is contained in the same directory.");
                }
                else
                {
                    std::wcout << "Successfully validated: " << FileHelper::GetModulePath() + L"Windows.AI.MachineLearning.dll" << std::endl;
                }
            }
            g_loadDllFromLocal = true;
        }
        else if ((_wcsicmp(args[i].c_str(), L"/?") == 0))
        {
            PrintUsage();
            return;
        }
    }

    if (m_modelPath.empty() && m_modelFolderPath.empty())
    {
        std::cout << std::endl;
        PrintUsage();
        return;
    }

    if (!m_inputData.empty())
    {
        if (m_inputData.find(L".png") != std::string::npos || m_inputData.find(L".jpg") != std::string::npos)
        {
            m_imagePath = m_inputData;
        }
        if (m_inputData.find(L".csv") != std::string::npos)
        {
            m_csvData = m_inputData;
        }
    }
}

