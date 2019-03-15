#include "filereader.h"

fileReader::fileReader(std::string filePath)
{
    //string "line" is used to read each line of config file
    std::string line;
    //input file stream
    std::ifstream infile;
    std::ofstream outfile;
    infile.open(filePath);
    outfile.open("../stickMan/uot.txt");
    int i=0;
    if(infile.is_open()){
        while(getline(infile,line)){
            switch(i){
                //Reads size on line 2
                case 2: if(line.compare("tiny")==0){m_size=0.5;}
                        else if(line.compare("normal")==0){m_size=1;}
                        else if(line.compare("large")==0){m_size=1.5;}
                        else if(line.compare("giant")==0){m_size=2;}
                        break;

                //Reads x coordinate on line 4
                case 4:m_xcoord = stoi(line);
                       break;

                //reads velocity on line 6
                case 6:m_velocity=stod(line);
                       break;

                //reads background image on line 8
                case 8:m_bgPath=line;
                       break;

                //reads frame width on line 10
                case 10: m_width=stoi(line);
                       break;
                //reads frame height on line 12
                case 12:m_heigth=stoi(line);
                        break;
            }
            i++;
        }
        infile.close();
    }
}
//Return background file path.
std::string fileReader::getBgPath(){
    return m_bgPath;
}
//return frame height
int fileReader::getHeigth(){
    return m_heigth;
}
//return frame width
int fileReader::getWidth(){
    return m_width;
}
//return man size
double fileReader::getSize(){
    return m_size;
}
//return velocity
double fileReader::getVelocity(){
    return m_velocity;
}
//return x coordinate
int fileReader::getX(){
    return m_xcoord;
}