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

// Класс для загрузки адресов из файла
class AddressListLoader
{
public:
    Address** readAddressList(std::ifstream& fin, int& count)
    {
        Address** res = nullptr;

        if (fin >> count)
        {
            bool addressReadError = false;
            res = new Address*[count]();
            for (int i = 0; i < count; ++i)
            {
                Address* addr = read(fin);
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

protected:
    Address* read(std::ifstream& fin)
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
};

// Класс, который сортирует, меняя порядок массива адресов на обратный
class AddressListReverseSorter
{
public:
    void process(Address** addrList, const int count)
    {
        for (int i = 0; i < count / 2; ++i)
        {
            std::swap(addrList[i], addrList[count - i - 1]);
        }
    }
};

// Класс для записи массива адресов в файл
class AddressListWriter
{
public:
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

private:
    bool writeAddress(std::ofstream& fout, Address* addr)
    {
        if (addr)
        {
            fout << addr->getCity() << ", " << addr->getStreet() << ", " << addr->getBuildingNumnber() << ", " << addr->getFlatNumber();
        }

        fout << std::endl;
        return static_cast<bool>(fout);
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    std::string inputFileName("in.txt");
    std::string outputFileName("out.txt");
    std::ifstream fin(inputFileName);

    if (fin.is_open())
    {
        auto loader = new AddressListLoader();
        int addrCount = 0;
        Address** addrList = loader->readAddressList(fin, addrCount);
        fin.close();
        if (addrList)
        {
            auto sorter = new AddressListReverseSorter();
            sorter->process(addrList, addrCount);

            std::ofstream fout(outputFileName);
            if (fout.is_open())
            {
                auto writer = new AddressListWriter();
                if (!writer->writeAddressList(fout, addrList, addrCount))
                {
                    std::cout << "Невозможно записать список адресов в файл '" << outputFileName << "'";
                }
                fout.close();
                loader->freeAddressList(addrList, addrCount);
            }
            else
            {
                std::cout << "Невозможно открыть файл '" << outputFileName << "'";
            }

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
