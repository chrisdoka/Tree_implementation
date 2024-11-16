/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Christina Doka
 */
#include <stdio.h>
#include <stdlib.h>

#include "pss.h"
struct Group G[MG];  // Πίνακας G MG θέσεων δήλωση global
struct SubInfo *S;   // Πρώτο στοιχείο λίστας συνδρομητών ..

static void groupArrayMatchingInit(int *igp, int* gids_arr,int size_of_gids_arr){   // Αρχικοποιση του πινακα με 0
    for(int i=0; i<MG;i++){
        igp[i]=0;
    }

    for(int i=0; i<size_of_gids_arr-1; i++){   // 1 στις θέσεις 
        igp[gids_arr[i]]=1;  
    }

}

static struct Info* create_new_info(int iId, int itm) { 
    struct Info *newInfo;                                //newInfo τύπου struct info
    newInfo= (struct Info *)malloc(sizeof(struct Info)); // δεύσμευση χώρου 
    if(newInfo==NULL){
        printf("error allocated info \n");              // έλεγχος εάν είναι κενή
        exit(1);
    }    
    newInfo->iId=iId;                                  //αρχικοποίση
    newInfo->itm=itm;
    newInfo->iprev=NULL;
    newInfo->inext=NULL;
    return newInfo;
}

static struct SubInfo* create_new_Sub(int sId, int stm){
      struct SubInfo *newSub;
   
    newSub=(struct SubInfo *) malloc (sizeof(struct SubInfo));
 
    if(newSub==NULL){
        printf("error allocating memory for newSub\n");
        exit(1);
    }
    for(int i=0; i<MG;i++){
        
       (newSub->sgp)[i]=(struct Info*)1;
    }
    newSub->sId=sId;
    newSub->stm=stm;
    newSub->snext=NULL;
   
    return newSub;
}

static struct Subscription* create_new_Sub2(int sId){
      struct Subscription *newSubr;
      
    newSubr=(struct Subscription*) malloc (sizeof(struct Subscription));
   
    if(newSubr==NULL){
        printf("error allocating memory for newSub\n");
        exit(1);
    }
    newSubr->sId=sId;
    newSubr->snext=NULL;
    return newSubr;
}

static void insertSub(struct SubInfo *newSub,int *gids_arr,int size_of_gids_arr){

    if(newSub==NULL){
    printf("error");
    exit(1);
    }
    if (S==NULL){
        S=newSub;
        return;
    }
        

    newSub->snext=S->snext;
    S->snext=newSub;

    for(int i=0; i <size_of_gids_arr-1 ; i++){
    
      (newSub->sgp)[gids_arr[i]]=G[gids_arr[i]].glast;
    }
    
}


static void insertSub2(int currgId2, struct Subscription *newSub){
    
    struct Subscription* head=G[currgId2].ggsub; 
    if(head==NULL){
        G[currgId2].ggsub=newSub;
        return;
    }
    
  newSub->snext=head->snext;
  head->snext=newSub;

}

static void insertInfo(int currgId, struct Info* new){       

    struct Info* head=G[currgId].gfirst;
    struct Info* tail=G[currgId].glast;
    struct Info*curr=head;
 
   if(head==NULL) {      // taksinomimeno me vasi itm
     G[currgId].gfirst=new;
     G[currgId].glast=new;
     return;
   }
   while(curr!=tail && curr->itm<=new->itm){
    curr=curr->inext;
   }
   if(curr->itm<=new->itm){
    if(curr==tail){
      tail->inext=new;  //new tail
      new->iprev=tail;
      new->inext=NULL;
     G[currgId].glast=new;
    }else{
      new->inext=curr;  //specific position 
      new->iprev=curr->iprev;
      curr->iprev->inext=new;
      curr->iprev=new;
    }
   } else{
     if(curr==head){  //new head
      new->inext=head;
      head->iprev=new;
      G[currgId].gfirst=new;
     }else{
      new->inext=curr;  //specific position 
      new->iprev=curr->iprev;
      curr->iprev->inext=new;
      curr->iprev=new;
     }
   }
   }

   

static void printInsertEvent(int* gids_arr,int size_of_gids_arr){
    struct Info* infoptr;
    for(int i=0 ; i<size_of_gids_arr-1; i++){

        printf("GROUPID=%d " , gids_arr[i]);
      infoptr=  G[gids_arr[i]].gfirst;
      printf(" INFOLIST=");
     while(infoptr!=NULL){
        printf("%d ",infoptr->iId) ;
       infoptr=infoptr->inext;
     }
     printf("\n");
        
    }

    printf("\n");
}

static void print_SubInfo(){
    struct SubInfo* start=S;
    printf("SUBSCRIBERLIST= ");
    while(start!=NULL){
        printf("%d ",start->sId);
        start=start->snext;
    }
    printf("\n");
}

static void printInsertSubscription(int* gids_arr,int size_of_gids_arr){
    struct Subscription* infoptr;
    for(int i=0 ; i<size_of_gids_arr-1; i++){

        printf("GROUPID=%d " , gids_arr[i]);
      infoptr=  G[gids_arr[i]].ggsub;
      printf(" INFOLIST=");
     while(infoptr!=NULL){
        printf("%d ",infoptr->sId) ;
       infoptr=infoptr->snext;
     }
     printf("\n");
        
    }

    printf("\n");
}

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void){
    for(int i=0; i<MG; i++){         //αρχικοποιήσεις 
        G[i].gId=i+1;
        G[i].ggsub=NULL;
        G[i].gfirst=NULL;
        G[i].glast=NULL;
    }
    S=NULL;
   
    return EXIT_SUCCESS;
}


/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
}


/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int groupId = 0;
    struct Info *newInfo = NULL;
    int currgId;
    
    for(int i=0; i<size_of_gids_arr-1; i++){
      
        newInfo = create_new_info(iId, iTM);
        groupArrayMatchingInit(newInfo->igp, gids_arr,size_of_gids_arr);
        currgId=gids_arr[i];
        insertInfo(currgId,newInfo) ; 
               
    }

printInsertEvent(gids_arr, size_of_gids_arr); 

    return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    struct SubInfo* newSub;
    struct Subscription* newSubr;
    int currgId2;
   
     newSub= create_new_Sub(sId,sTM);
    insertSub(newSub,gids_arr,size_of_gids_arr);
     
    for(int i=0; i<size_of_gids_arr-1; i++){
    newSubr=create_new_Sub2(sId);
     currgId2=gids_arr[i];
    insertSub2(currgId2 , newSubr);

    }
    print_SubInfo();
     printInsertSubscription(gids_arr,size_of_gids_arr);
    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
    int i=0;
    struct SubInfo *currentSub=S;
    struct Info *eachGroup;
    struct Subscription *groupFinder;
 
    while(currentSub!=NULL){
    if(currentSub->sId==sId ){
        break;
    }
    currentSub=currentSub->snext;
    }
    

for(int i=0; i <MG; i++){
   groupFinder= G[i].ggsub;
   while(groupFinder!=NULL){
    if(groupFinder->sId==sId){
        printf("GROUPID= %d, INFOLIST = ",i);
      eachGroup=G[i].gfirst;
        while(eachGroup!=NULL){
            printf("%d ",eachGroup->iId);
            eachGroup=eachGroup->inext;
        }
        printf("\n");
      
    }
    groupFinder=groupFinder->snext;
   }
}

    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */

void deleteSub(int sId){
    struct SubInfo* current=S;
    struct SubInfo* prev;
    prev=NULL;

    while(current!=NULL){
       if (current->sId==sId){
        if(current==S){
            S=current->snext;
            free(current);
            return;
        }
        prev->snext=current->snext;
        free(current);
        return;
       }
       prev=current;
       current=current->snext;
    }
}

void deleteSub2(int sId){
    
int savedGroups[MG]={0};
for(int i=0; i<MG; i++){
    savedGroups[i] = -1;
}
int index=0;
 for(int i=0; i<MG; i++){
    struct Subscription* prev;
  prev=NULL;
   struct Subscription* current=G[i].ggsub;
     while(current!=NULL){
       if (current->sId==sId){
        savedGroups[index]=i;
        index=index+1;
        if(current==G[i].ggsub){
           
            G[i].ggsub=current->snext;
            free(current);
            break;
        }
          prev->snext=current->snext;
        free(current);
        break;
       }
        prev=current;
       current=current->snext;
    }
 }


    for(int i=0;i<MG;i++){
      if(savedGroups[i]!=-1){
   
        int groupId=savedGroups[i];
         printf("GROUPID=%d, SUBLIST= ", groupId);
        struct Subscription* start= G[groupId].ggsub;
        while(start!=NULL){
        
            printf("%d ",start->sId);
            start=start->snext;
        }
        printf("\n");
      }
    }
 }

    


int Delete_Subscriber(int sId){
    deleteSub(sId);
    deleteSub2(sId);
      print_SubInfo();
    
    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    struct Info* infoptr;
    int counterGroups=0;
    int counterSub=0;
    for(int i=0 ; i<64; i++){

        printf("GROUPID=%d " , i );

        
      infoptr=  G[i].gfirst;
      printf(" INFOLIST=");
     while(infoptr!=NULL){
        printf("%d ",infoptr->iId) ;
       counterGroups=counterGroups+1; // counter for groups
       infoptr=infoptr->inext;
     }
     printf("\n");
    }
   struct SubInfo* start=S;
    printf("SUBSCRIBERLIST= ");
    while(start!=NULL){
        printf("%d ",start->sId);
        counterSub=counterSub+1;
        start=start->snext;
    }
    printf("\n");
   

   

 struct SubInfo *p=S;

while(p!=NULL){
    printf("SUBSCRIBER=%d ",p->sId);
   printf("GroupId=");
    for(int i=0;i<MG;i++){
       if(p->sgp[i]!=(struct Info*)1){
        printf("%d ",i);
       }
    }
    p=p->snext;
    printf("\n");

}
 printf("NO_GROUPS= ");
 printf("%d",counterGroups);
 printf("\n");
 printf("NO_SUBSCRIBERS= ");
 printf("%d",counterSub);
  printf("\n");
 return EXIT_SUCCESS;  
}
