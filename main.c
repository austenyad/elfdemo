//
// Created by austenYang on 2020/7/10.
//

#include <stdio.h>
#include "elf.h"
#include "malloc.h"


void checkElffile(FILE *file);

void printfElf64_Message(FILE *file);

void printf_Elf64_Ehdr_Message(FILE *file, unsigned int offset, unsigned int count, uint16_t shstrndx);

void printf_Elf64_Shadr_Message(FILE *file, long offset, unsigned int count, uint16_t shstrndx);

void runElfParse(void) {
    FILE *file = NULL;
    file = fopen("../libhello.so", "rb");
    if (!file) {
        printf("Open file fail\n");
        return;
    } else {
        checkElffile(file);
    }

    fclose(file);
}

void checkElffile(FILE *file) {
    if (!file) {
        return;
    }
    unsigned char buff[16];
    rewind(file);
    if (1 == fread(buff, 16 * sizeof(char), 1, file)) {
        if (buff[EI_MAG0] == ELFMAG0 &&
            buff[EI_MAG1] == ELFMAG1 &&
            buff[EI_MAG2] == ELFMAG2 &&
            buff[EI_MAG3] == ELFMAG3) {
            switch (buff[EI_CLASS]) {
                case ELFCLASSNONE:
                    printf("FILE");
                    break;
                case ELFCLASS32:
                    printf("32");
                    break;
                case ELFCLASS64:
                    printf("ELF36 --\n");
                    printfElf64_Message(file);
                    break;
            }
        }
    }

}

void printfElf64_Message(FILE *file) {
    rewind(file);
    Elf64_Ehdr elf64Ehdr;
    if (1 == fread(&elf64Ehdr, sizeof(Elf64_Ehdr), 1, file)) {
        /* Legal values for e_type (object file type).  */
        printf("Header------------\n");
        printf("文件类型：");
        switch (elf64Ehdr.e_type) {
            case ET_NONE:
                printf(" No file type  -- 没有文件类型！");
                break;
            case ET_REL:
                printf("Relocatable file -- 可重定位文件");
                break;
            case ET_EXEC:
                printf("Executable file  -- 可执行文件");
                break;
            case ET_DYN:
                printf("Shared object file -- 共享目标文件");
                break;
            case ET_CORE:
                printf("Core file -- 核心转储文件");
                break;
        }
        //Legal values for e_machine (architecture).
        printf("\n机器类型：");
        switch (elf64Ehdr.e_machine) {
            case EM_X86_64:
                printf("AMD x86-64 architecture(Advanced Micro Devices X86-64)");
                break;
        }

        printf("\nELF 版本号：");
        switch (elf64Ehdr.e_version) {
            case EV_NONE:
                printf("Invalid ELF version");
                break;
            case EV_CURRENT:
                printf("Current version ");
                break;
            case EV_NUM:
                printf("");
                break;
        }
        //TODO 后面将其做成表
        printf("\n入口地址：0x%llx", elf64Ehdr.e_entry);

        //Program header table file offset
        printf("\nStart of program headers：%lld", elf64Ehdr.e_phoff);


        printf("\n段表（section table）在文件中的偏移：%lld", elf64Ehdr.e_shoff);
        printf("\n处理器特殊标记 Flags ：0x%x", elf64Ehdr.e_flags);
        printf("\nELF文件 head 占用: %d 字节", elf64Ehdr.e_ehsize);
        printf("\n程序头条目大小：%d 字节", elf64Ehdr.e_phentsize);
        printf("\n程序头条目数：%d 个", elf64Ehdr.e_phnum);
        printf("\n段表描述符的大小：%d 字节", elf64Ehdr.e_shentsize);
        printf("\n段表描述符数量：%d 个", elf64Ehdr.e_shnum);
        //TODO 含义不清
        printf("\n段表字符串表所在的段在段表中的下标：%d ", elf64Ehdr.e_shstrndx);


//        rewind(file);
//        fseek(file, sizeof(Elf64_Ehdr), SEEK_SET);
//        Elf64_Shdr elf64Shdr[elf64Ehdr.e_shnum];
//
//        size_t reslut = fread(&elf64Shdr, sizeof(Elf64_Shdr), elf64Ehdr.e_shnum, file);
//        if (reslut == 1) {
//            printf("\nSuccess!");
//        } else {
//            printf("Error!");
//        }
        printf_Elf64_Shadr_Message(file, elf64Ehdr.e_shoff, elf64Ehdr.e_shnum, elf64Ehdr.e_shstrndx);
    }


}

/**
 *
 */
void printf_Elf64_Shadr_Message(FILE *file, long offset, unsigned int count, uint16_t shstrndx) {
    fseek(file, offset, SEEK_SET);
    printf("Section header from %ld bytes \n", ftell(file));

    Elf64_Shdr *shdr = malloc(count * sizeof(Elf64_Shdr));

    if (count == fread(shdr,sizeof(Elf64_Shdr),count,file)){
        printf("\t[%2s]\t%-24s%-12s\t%-8s\t%-6s\t%-6s\t%2s\t%3s\t%2s\t%3s\t%2s\n",
               "Nr", "Name", "Type", "Addr", "Off", "Size", "ES", "Flg", "Lk", "Inf", "Align");
    }

}

int main() {
    runElfParse();
    return 0;
}


