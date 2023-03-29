#ifndef SOURCE_H
#define SOURCE_H

class Source
{
public:
    Source(int total, int available);
    int total;     // 资源总量
    int available; // 资源尚未分配的量
};

#endif // SOURCE_H
