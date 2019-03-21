/**
* TCP��� ճ������
* @author mengdj@outlook.com
*/
#ifndef PAC_H
#define PAC_H
#ifndef INNER_BUFFER_SIZE
#define INNER_BUFFER_SIZE 10240         //�ڲ��������Ĵ�С
#endif
#include <memory.h>
#include <queue>
typedef unsigned char byte;
typedef struct _PAH_DESC{
    int length;
    int loop;
    byte* buffer;
} pac_desc;

//�����У�����Ѿ��𿪵İ���
static std::queue<pac_desc*> pendingQueue;
//�ڲ�������(�ֱ������������Լ���ǰ��������)
static pac_desc *pr=NULL,*pd=NULL;

//intת��Ϊ�ֽ�
static void int2byte(int i,byte* bytes,int size=4){
    memset(bytes,0,sizeof(byte)*size);
    bytes[0]=(unsigned char)(0xff&i);
    bytes[1]=(unsigned char)((0xff00&i)>>8);
    bytes[2]=(unsigned char)((0xff0000&i)>>16);
    bytes[3]=(unsigned char)((0xff000000&i)>>24);
}

//�ֽ�ת��Ϊint
static int byte2int(byte* bytes,int size=4){
    int addr=bytes[0]&0xFF;
    addr|=((bytes[1]<<8)&0xFF00);
    addr|=((bytes[2]<<16)&0xFF0000);
    addr|=((bytes[3]<<24)&0xFF000000);
    return addr;
}

/**
* ��ʼ��
*/
void packet_init(){
}

//�ͷŵ�����Դ
void packet_recycle(pac_desc *pd){
    if(pd!=NULL){
        if(pd->buffer!=NULL){
            free(pd->buffer);
        }
        free(pd);
    }
}

/**
* �ͷ���Դ�����ٶ���
*/
void packet_destory(){
     while(!pendingQueue.empty()){
        pac_desc* pt=(pac_desc*)pendingQueue.front();
        if(pt!=NULL){
            free(pt->buffer);
            free(pt);
        }
        pendingQueue.pop();
    }

    if(pr!=NULL){
        free(pr->buffer);
        free(pr);
        pr=NULL;
    }

    if(pd!=NULL){
        free(pd->buffer);
        free(pd);
        pd=NULL;
    }
}

//��ȡһ����
pac_desc* packet_out(){
    if(pendingQueue.empty()){
        return NULL;
    }
    pac_desc* pt=(pac_desc*)pendingQueue.front();
    pendingQueue.pop();
    return pt;
}

/**
* ��ȡ���д�С
*/
int packet_size(){
    if(pendingQueue.empty()){
        return 0;
    }
    return pendingQueue.size();
}

/**
* ��������ȺͲ����4�ֽڣ�������
* buffer:����ָ��
* size:ָ�볤��
*/
void packet_in(byte* buffer,int size){
     if(pr!=NULL){
        if(pr->loop){
            //�ϲ�������,���Ƶ���������ָ���µĻ���������СΪ�ϲ���Ĵ�С(�ڲ�������������ڰ��Ĵ�С��������һ���ڴ�)
            memcpy(pr->buffer+pr->loop,buffer,size);
            buffer=pr->buffer;
            size+=pr->loop;
            pr->loop=0;
        }
     }
     if(!size){
        return;
     }
     if(pd==NULL){
        pd=(pac_desc*)malloc(sizeof(pac_desc));
        pd->length=byte2int(buffer);
        pd->buffer=(byte*)malloc(pd->length);
        pd->loop=0;
        //����ָ������ݵĳ���
        buffer+=4;
        size-=4;
     }

     if(size>0){
         int cond=pd->loop+size;
         printf("%d\r\n",cond);
         if(cond>=pd->length){
            if(cond==pd->length){
                //�պ�һ����
                memcpy(pd->buffer+pd->loop,buffer,size);
                pd->loop+=size;
                size=0;
            }else{
                //һ�������ж����
                int remain=(pd->length-pd->loop);
                memcpy(pd->buffer+pd->loop,buffer,remain);
                pd->loop+=remain;
                buffer+=remain;
                size-=remain;
            }
            if(pd->loop==pd->length){
                int pdh=pd->length;
                pendingQueue.push(pd);
                pd=NULL;
                if(size!=0){
                    //�ж����
                    if(size<4){
                        //������ͷ�ĳ��ȣ���Ҫ���õڶ���������
                        if(pr==NULL){
                            pr=(pac_desc*)malloc(sizeof(pac_desc));
                            //����һ���㹻����ڴ滺����
                            pr->buffer=(byte*)malloc(INNER_BUFFER_SIZE);
                        }
                        pr->length=size;
                        pr->loop=size;
                    }else{
                        packet_in(buffer,size);
                    }
                }
            }
         }else{
            memcpy(pd->buffer+pd->loop,buffer,size);
            pd->loop+=size;
         }
     }
}

cout << "ok?" << endl;
cout << "lucky" << endl;
cout << "no buggy day" << endl;
cout << "the fifth personal" << endl;
cout << "be quickly..." << endl;

#endif