#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED
template <class T>
class Container{
private:
    T* contents = nullptr;
    int num = 0;

public:

    void add(T element){
        if(contents == nullptr){
            contents = new T[1];
            contents[0] = element;
            num = 1;
        }

        //copy entire old contents to new contents, then assign the element:
        T* newContents = new T[num+1];
        for(int i = 0; i < num; i++){
            newContents[i] = contents[i];
        }
        newContents[num] = element;
        //increase size:
        num++;
        //Delete old contents and assign new one:
        delete[] contents;
        contents = newContents;
    }

    bool remove(T element){
        if(contents == nullptr){
            return false;
        }
        if(num == 1){
            num = 0;
            contents = nullptr;
            return true;
        }

        //copy entire old contents to new contents, except the one to be removed:
        T* newContents = new T[num-1];
        int j=0;
        for(int i = 0; i < num; i++){
            if(contents[i] == element){
                //skip element with same contents as the one being removed
                //this means j won't increase and thus will end up being less than i
                continue;
            }
            if(j == num-1){
                delete[] newContents;
                return false;
            }
            newContents[j] = contents[i];
            j++;
        }
        //decrease size:
        num--;
        //Delete old contents and assign new one:
        delete[] contents;
        contents = newContents;
        return true;
    }

    int getSize(){
        return num;
    }

    T* get(int index){
        if(index >= num || index < 0)
            return nullptr;
        return contents+index;
    }

    void removeAll(){
        delete[] contents;
        contents = nullptr;
        num = 0;
    }
};


#endif // CONTAINER_H_INCLUDED
