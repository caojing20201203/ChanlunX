#ifndef __LEVEL_H
#define __LEVEL_H

using namespace std;
#pragma pack(push, 1)

enum class Level_No {MONTH, WEEK, DAY, MIN30, MIN5, MIN1};
class Level {
    private:
        Level_No no;
    public:
        Level() {
            this->no = Level_No::DAY;
        }
        
        Level(Level_No no){
            this->no = no;
        }

        void upgrade() {
            switch (this->no) {
                case Level_No::MONTH:
                    break;
                case Level_No::WEEK:
                    this->no = Level_No::MONTH;
                    break;
                case Level_No::DAY:
                    this->no = Level_No::WEEK;
                    break;
                case Level_No::MIN30:
                    this->no = Level_No::DAY;
                    break;
                case Level_No::MIN5:
                    this->no = Level_No::MIN30;
                    break;
                case Level_No::MIN1:
                    this->no = Level_No::MIN5;
                    break;
            }
        }
};

#endif