#include <iostream>
#include <fstream>

class Address
{
public:
    Address(const std::string& city, const std::string& street, const int buildingNumber, const int flatNumber)
    {
        _city = city;
        _street = street;
        _buildingNumber = buildingNumber;
        _flatNumber = flatNumber;
    }

    std::string& getCity()
    {
        return _city;
    }

    void setCity(std::string& city)
    {
        _city = city;
    }

    std::string& getStreet()
    {
        return _street;
    }

    void setStreet(std::string& street)
    {
        _street = street;
    }

    int getBuildingNumnber()
    {
        return _buildingNumber;
    }

    void setBuildingNumber(int buildingNumber)
    {
        _buildingNumber = buildingNumber;
    }

    int getFlatNumber()
    {
        return _flatNumber;
    }

    void setFlatNumber(int flatNumber)
    {
        _flatNumber = flatNumber;
    }

private:
    std::string _city;
    std::string _street;
    int _buildingNumber;
    int _flatNumber;
};

namespace AddressList
{
    void freeAddressList(Address** addrList, int count)
    {
        if (addrList)
        {
            for (int i = 0; i < count; ++i)
            {
                if (addrList[i])
                {
                    delete addrList[i];
                }
            }

            delete[] addrList;
        }
    }

    Address* readAddress(std::ifstream& fin)
    {
        Address* res = nullptr;
        std::string city;
        std::string street;
        int buildingNumber;
        int flatNumber;
        fin >> city;
        fin >> street;
        fin >> buildingNumber;
        fin >> flatNumber;

        if (fin)
        {
            res = new Address(city, street, buildingNumber, flatNumber);
        }

        return res;
    }

    bool writeAddress(std::ofstream& fout, Address* addr)
    {
        if (addr)
        {
            fout << addr->getCity() << ", " << addr->getStreet() << ", " << addr->getBuildingNumnber() << ", " << addr->getFlatNumber();
        }

        fout << std::endl;
        return static_cast<bool>(fout);
    }

    Address** readAddressList(std::ifstream& fin, int& count)
    {
        Address** res = nullptr;

        if (fin >> count)
        {
            bool addressReadError = false;
            res = new Address * [count]();
            for (int i = 0; i < count; ++i)
            {
                Address* addr = readAddress(fin);
                res[i] = addr;
                if (!addr)
                {
                    addressReadError = true;
                }
            }

            if (addressReadError)
            {
                freeAddressList(res, count);
                res = nullptr;
            }
        }

        return res;
    }

    int compare(Address* addr1, Address* addr2)
    {
        if (addr1 == addr2)
        {
            return 0;
        }

        if (!addr1)
        {
            return -1;
        }

        if (!addr2)
        {
            return 1;
        }

        return addr1->getCity().compare(addr2->getCity());
    }

    void sortAddressList(Address** addrList, const int count)
    {
        // взял обратный пузырек из одного из прошлых заданий
        bool swapped;
        do {
            swapped = false;
            for (int i = count - 1; i > 0; --i) {
                if (compare(addrList[i], addrList[i - 1]) < 0) {
                    swapped = true;
                    Address* tmp = addrList[i];
                    addrList[i] = addrList[i - 1];
                    addrList[i - 1] = tmp;
                }
            }
        } while (swapped);
    }

    bool writeAddressList(std::ofstream& fout, Address** addrList, const int count)
    {
        fout << count << std::endl;
        if (fout)
        {
            for (int i = 0; i < count; ++i)
            {
                if (!writeAddress(fout, addrList[i]))
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");

    std::string inputFileName("in.txt");
    std::string outputFileName("out.txt");
    std::ifstream fin(inputFileName);

    if (fin.is_open())
    {
        int addrCount = 0;
        Address** addrList = AddressList::readAddressList(fin, addrCount);
        fin.close();
        if (addrList)
        {
            AddressList::sortAddressList(addrList, addrCount);

            std::ofstream fout(outputFileName);
            if (fout.is_open())
            {

                if (!AddressList::writeAddressList(fout, addrList, addrCount))
                {
                    std::cout << "Невозможно записать список адресов в файл '" << outputFileName << "'";
                }
                fout.close();
            }
            else
            {
                std::cout << "Невозможно открыть файл '" << outputFileName << "'";
            }

            AddressList::freeAddressList(addrList, addrCount);
        }
        else
        {
            std::cout << "Не удалось прочитать список адресов из файла '" << inputFileName << "'";
        }
    }
    else
    {
        std::cout << "Невозможно открыть файл '" << inputFileName << "'";
    }

}
