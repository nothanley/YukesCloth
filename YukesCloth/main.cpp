// YukesCloth.cpp : This file contains the 'main' function. Program execution begins and ends there. //
#include "YukesCloth"
#include <iostream>

int main()
{

    CClothContainer _yclContainer(
        "C:\\Users\\wauke\\Desktop\\hair_10083.ycl");


    CClothSave::SaveToDisk(
        "C:\\Users\\wauke\\Desktop\\out_file.ycl", 
        _yclContainer.m_pClothSimObj);
}
