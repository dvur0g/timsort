#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>

int getMinrun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

template<typename T>
void printArray(T a)
{
    for(int j = 0; j < a.size(); j++)
        std::cout << a[j] << " ";
    std::cout << "\n";
}

struct run
{
    int index;
    int length;

    run(int i, int l)
    {
        index = i;
        length = l;
    }
};

int search(int val, int* a, int length)
{
    int low = 0, high = length - 1, mid;
    mid = (low + high)/2;
    while(low <= high)
    {   
        if(a[mid] == val)
            return mid;
        if(a[mid] > val)
            high = mid - 1;
        else
            low = mid + 1;
        mid = (low + high)/2;
    }
    if(mid == 0)
        return -1;
    return mid;
}

int searchBack(int val, int* a, int length)
{
    int low = 0, high = length - 1, mid;
    mid = (low + high)/2;
    while(low <= high)
    {   
        if(a[mid] == val)
            return mid;
        if(a[mid] < val)
            low = mid + 1;
        else
            high = mid - 1;
        mid = (low + high)/2;
    }
    if(mid == length - 1)
        return -1;
    return (a[mid] > val) ? mid : mid + 1;
}

int gallop(int* b, int &j, int &lenb, int* a, int &i, int &lena, int* res, int &k, int &lenres)
{
    int countForward = i, countBack;

    int pos = search(b[j], a + i, lena - i);
    if(pos >= 0)
    {    
        int enda = i + pos + 1;
        while(i < enda)
        {
            res[k++] = a[i++];
            
        }
        res[k++] = b[j++];

        countForward = enda - countForward;
    }

    if(i >= lena)
        return countForward;

    int find = b[lenb - 1];
    pos = searchBack(find, a + i, lena - i);
    if(pos >= 0)
    {   
        pos += i;
            
        int l = lena - 1, m = lenres - 1;

        countBack = l;
        
        while(l >= pos)
            res[m--] = a[l--];
        res[m] = b[--lenb];
        
        lena -= (lena - pos);
        lenres = m;

        countBack -= pos;
    }
    return countForward + countBack;
}

void merge(int* a, int lena, int* b, int lenb)
{
    int minThreshold = 7;
    int threshold = 0;

    int lenres = lena + lenb, LEN_RES = lenres;
    int* res = new int[lenres];
    
    int i = 0, j = 0, k = 0; 
    while (i < lena && j < lenb) 
    {
        if (a[i] < b[j])
        { 
            res[k++] = a[i++];
    
            if(threshold >= 0)
                threshold++;
            else
                threshold = 0;
        }
        else
        {
            res[k++] = b[j++];
            


            if(threshold <= 0)
                threshold--;
            else
                threshold = 0;
        }

        if(threshold == minThreshold)
        {
            if(gallop(b, j, lenb, a, i, lena, res, k, lenres) < minThreshold)
                minThreshold++;
            else
                minThreshold--;
            threshold = 0;
        }
        else if(threshold == -minThreshold)
        {
            if(gallop(a, i, lena, b, j, lenb, res, k, lenres) < minThreshold)  
                minThreshold++;
            else 
                minThreshold--;
            threshold = 0;
        }
    } 

    while (i < lena)
        res[k++] = a[i++]; 
    
    while (j < lenb) 
        res[k++] = b[j++]; 
    
    for(int i = 0; i < LEN_RES; i++)
         a[i] = res[i];
}

std::vector<int> sort(std::vector<int> a)
{
    std::vector<run> mergea; 

    int length = a.size();
    int minrun = getMinrun(length);
    
    int i = 0, check, checkReverse, checkIncrease;
    
    while(i + 1 < length)
    {
        check = i;
        if(a[i] <= a[i + 1])
        {
            checkIncrease = i;
            while(a[i] <= a[i + 1])
                i++;
                
            for(; i < check + minrun && i + 1 < length; i++)
            {
                if(a[i] <= a[i + 1])
                    continue;
                else
                    for(int j = i; a[j + 1] < a[j] && j >= check; j--)
                        std::swap(a[j + 1], a[j]);
            }
            mergea.push_back({checkIncrease, i - checkIncrease + 1});
        }
        else
        {
            checkReverse = i;
            while(a[i] > a[i + 1])
                i++;
               
            for(; i < check + minrun && i + 1 < length; i++)
            {
                
                if(a[i] > a[i + 1])
                    continue;
                else
                    for(int j = i; a[j + 1] > a[j] && j >= check; j--)
                        std::swap(a[j + 1], a[j]);
                
            }
            for(int low = checkReverse, high = i; low < high; low++, high--)
                std::swap(a[low], a[high]);

            mergea.push_back({checkReverse, i - checkReverse + 1});
        }
        i++;
    }
 
    
    while(mergea.size() > 1)
    {
        for(int i = 0; i + 1 < mergea.size(); i++)
        { 
            if(i == mergea.size() - 2)
            {
                merge(&a[mergea[i].index], mergea[i].length, &a[mergea[i + 1].index], mergea[i + 1].length);

                mergea.insert(mergea.begin() + i, {mergea[i].index, mergea[i].length + mergea[i + 1].length});
                    
                mergea.erase(mergea.begin() + i + 1);
                mergea.erase(mergea.begin() + i + 1);

                break;
            }

            if(mergea[i].length > mergea[i + 1].length + mergea[i + 2].length && mergea[i + 1].length > mergea[i + 2].length)
                continue;
            else
            {
                if(mergea[i].length <= mergea[i + 2].length)
                {  
                    merge(&a[mergea[i].index], mergea[i].length, &a[mergea[i + 1].index], mergea[i + 1].length);

                    mergea.insert(mergea.begin() + i, {mergea[i].index, mergea[i].length + mergea[i + 1].length});
                    
                    mergea.erase(mergea.begin() + i + 1);
                    mergea.erase(mergea.begin() + i + 1);
                }
                else
                {
                    merge(&a[mergea[i + 1].index], mergea[i + 1].length, &a[mergea[i + 2].index], mergea[i + 2].length);
                    
                    mergea.insert(mergea.begin() + i + 1, {mergea[i + 1].index, mergea[i + 1].length + mergea[i + 2].length});
                    
                    mergea.erase(mergea.begin() + i + 2);
                    mergea.erase(mergea.begin() + i + 2);
                }
            }
        }
    }
    return a;
}

int main()
{
    int bias = 48;
    std::string inputString;

    std::cout << "Input numbers separated with spaces (4094 characters max):\n";
    std::getline(std::cin, inputString);
    
    std::string numberString;
    std::vector<int> array;

    bool sign = true;
    for(int i = 0; i < inputString.length(); i++)
    {
        if(inputString[i] == '-')
        {
            sign = false;
            i++;
        }
        while(inputString[i] >= bias && inputString[i] <= 9 + bias && i < inputString.length())
        {
            numberString.push_back(inputString[i]);
            i++;
        }
        if(numberString.length())
        {
            if(sign == false)
                array.push_back(-stoi(numberString));
            else
                array.push_back(stoi(numberString));
            numberString.clear();
        }
        sign = true;
    }

    array = sort(array);
    printArray(array);

    return 0;
}