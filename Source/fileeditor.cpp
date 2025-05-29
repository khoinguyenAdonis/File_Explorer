#include "Header/fileeditor.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <QFile>
#include <QTextStream>

fileEditor::fileEditor(QObject *parent)
    : QObject{parent}
{}

QString fileEditor::contents() const
{
    return m_contents;
}

void fileEditor::setContents(const QString &newContents)
{
    if (m_contents == newContents)
        return;
    m_contents = newContents;
    emit contentsChanged();
}

void fileEditor::readFileToRam(const QString &filePath)
{
    QByteArray pathBytes = filePath.toUtf8();
    char* c_path = pathBytes.data();

    int fd = open(c_path, O_RDONLY);
    if (fd == -1){
        emit fileReadError("Cant't read file" + filePath);
        close(fd);
        return;
    }

    struct stat st;
    if(fstat(fd,&st) == -1){
        emit fileReadError("Cant't read information file" + filePath);
        close(fd);
        return;
    }

    int fileSize = st.st_size;
    char* mapped_memory = static_cast<char*>(mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));
    if (mapped_memory == MAP_FAILED) {
        std::cerr << "Lỗi mmap." << std::endl;
        close(fd);
        return;
    }

    close(fd);
    QString myQString = QString::fromUtf8(mapped_memory);

    emit fileContentReady(myQString);
    munmap(c_path,fileSize);
}
