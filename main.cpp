#include <QCoreApplication>
#include <cmath>
#include <iostream>
#include <fstream>


void writeFile()
{
    std::ofstream myfile;
    myfile.open ("/Users/sabrinaheppner/Documents/workspace_Qt/readData/example2.txt");
    if (myfile.is_open())
    {
       // myfile << "This is my new file. \nThis is a line.\n";
       // myfile << "This is another line.\n";
        myfile << "Test";
        std::cout << "writing into file " << std::endl;
        myfile.close();
    }else{
        std::cerr << "NOT writing into file " << std::endl;
    }
}

void readFile()
{

    std::string line;
    std::ifstream myfile ("/Users/sabrinaheppner/Documents/workspace_Qt/readData/data.txt");
     if (myfile.is_open())
     {
       std::cout << "reading from file data.txt:\n" << std::endl;
       while (std::getline (myfile,line) )
       {
         std::cout << line << '\n';
       }
       myfile.close();
       std::cout<< std::endl;
     }

     else std::cout << "Unable to open file";
}

char* readByteFile()
{
    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen("/Users/sabrinaheppner/Documents/workspace_Qt/readData/heart.raw", "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file

    std::cout << "byte offset in the file:"<<filelen << "\n"<< std::endl;

    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file

    //std::cout << "file data:"<< buffer << "\n"<< std::endl;

    fclose(fileptr); // Close the file

    return buffer;
}

void getFileSize()
{
    std::streampos begin,end;
    std::ifstream myfile ("/Users/sabrinaheppner/Documents/workspace_Qt/readData/data.txt");
    if (myfile.is_open())
    {
        begin = myfile.tellg();
        myfile.seekg (0, std::ios::end);
        end = myfile.tellg();
        myfile.close();
        std::cout << "size is: " << (end-begin) << " bytes.\n"<<std::endl;
    }
    else std::cout << "Unable to open file"<<std::endl;



}




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    writeFile();
   // readFile();
    //getFileSize();
    char *buffer = readByteFile();
    //std::cout << "file data:"<< buffer << "\n"<< std::endl;
    int s = sizeof(buffer);
    std::cout << "size:"<< s << "\n"<< std::endl;

    int XDIM = 64;
    int YDIM = 64;
    int ZDIM = 64;

    char img[XDIM][YDIM][ZDIM];


    for (int x = 0; x < XDIM; x++) {
        for (int y = 0; y < YDIM; y++) {
            char c = buffer[(x + (XDIM * y) + (5 * XDIM * YDIM))];
            img[x][y][0] = c;
            img[x][y][1] = c;
            img[x][y][2] = c;
            img[x][y][3] = c;

            std::cout << "c:"<< c << "\n"<< std::endl;
        }
    }




    return a.exec();
}
