#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define dim  1000
#define reportDim 400

typedef enum {
    false, true
} bool;


typedef char nameID[40];

typedef struct entityPunt {
    nameID *entityName;
    bool deleted;
    int position;
};

typedef struct entityData {
    nameID name;
    int puntPos;
};

typedef nameID **sender;//puntatore a puntatore
typedef struct receiver {
    nameID **id; // puntatore a puntatore
    sender *senders;// array di senders
    int sendersSize;
};
typedef struct report {
    struct receiver *maxReceiver;//array dei riceventi max
    int max;
    int reciversize;
};

typedef struct relation {
    nameID id;
    int receiversSize;
    struct receiver *receivers;//array di receiver
    struct report report;
};

int findReportReceiver(struct relation *relation, nameID toFind, int size) {
    //scorrere sender e tornare la pos se esiste altrimenti tornare -1

    int inf = 0, sup = size - 1;
    while (inf <= sup) {
        int med = ((inf + sup) / 2);
        if (strcmp(**relation->report.maxReceiver[med].id, toFind) == 0) {
            return med; // nel caso passare la posizione
        }
        if (strcmp(toFind, **relation->report.maxReceiver[med].id) > 0) {
            inf = med + 1;
        } else {
            sup = med - 1;
        }
    }
    return -1;
}

int findNewMaxReceivers(struct relation *relation) {
    relation->report.reciversize = 0;
    relation->report.max = 1;
    free(relation->report.maxReceiver);
    //si trova nuovo max
    for (int i = 0; i < relation->receiversSize; ++i) {

        if (relation->report.max < relation->receivers[i].sendersSize) {
            relation->report.max = relation->receivers[i].sendersSize;
        }
    }
    relation->report.maxReceiver = malloc(sizeof(struct receiver) * reportDim);
    //si trovano i max receiver
    for (int j = 0; j < relation->receiversSize; ++j) {
        if (relation->report.max == relation->receivers[j].sendersSize) {
            if (relation->report.reciversize % reportDim == 0) {
                relation->report.maxReceiver = realloc(relation->report.maxReceiver, sizeof(struct receiver) *
                                                                                     (reportDim +
                                                                                      relation->report.reciversize));
            }
            relation->report.maxReceiver[relation->report.reciversize].id = relation->receivers[j].id;
            relation->report.reciversize++;
        }
    }
    return relation->report.reciversize;
}

void remakeReport(struct relation *relation, nameID receiverToRemove) {
    int reportPos = findReportReceiver(relation, receiverToRemove,
                                       relation->report.reciversize);

    if (reportPos != -1) {//receiver presente nel report
        if (relation->report.reciversize == 1) {//se era il massimo ed era unico
            relation->report.reciversize = findNewMaxReceivers(relation);//si ricalcolano i max sender
        } else {
            //caso in cui è il massimo insieme ad altri elementi
            for (int i = reportPos; i < relation->report.reciversize - 1; ++i) {
                relation->report.maxReceiver[i] = relation->report.maxReceiver[i + 1];
            }
            relation->report.reciversize--;
        }

    }
}

void updateReport(struct relation *relation, struct receiver *lastAdd) {
    if (lastAdd->sendersSize ==
        relation->report.max)// se il numero delle relazioni diventa uguale al massimo bisogna aggiungere il receiver
    {
        if (relation->report.reciversize == 0) {
            relation->report.maxReceiver = malloc(sizeof(struct receiver) * reportDim);
        }
        if (relation->report.reciversize != 0 && (relation->report.reciversize % reportDim) == 0) {
            relation->report.maxReceiver = realloc(relation->report.maxReceiver, sizeof(struct receiver) * (reportDim +
                                                                                                            relation->report.reciversize));
        }

        int receiverPos = 0;
        while (receiverPos < relation->report.reciversize &&
               strcmp(**relation->report.maxReceiver[receiverPos].id, **lastAdd->id) < 0) {
            receiverPos++;
        }
        for (int i = relation->report.reciversize - 1; i >= receiverPos; i--) {
            relation->report.maxReceiver[i + 1] = relation->report.maxReceiver[i];
        }

        relation->report.maxReceiver[receiverPos].id = lastAdd->id;
        relation->report.reciversize++;
    }

    if (lastAdd->sendersSize > relation->report.max) {//se il numero delle relazioni è maggiore allora sarà la sola
        free(relation->report.maxReceiver);
        relation->report.maxReceiver = malloc(sizeof(struct receiver) * reportDim);
        relation->report.maxReceiver[0].id = lastAdd->id;
        relation->report.max = lastAdd->sendersSize;
        relation->report.reciversize = 1;
    }
}

int findRelation(struct relation *relations, nameID toFind, int size) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(relations[i].id, toFind) == 0) {
            return i;
        }
    }
    return size; // si deve creare la relazione
};

bool searchEntity(nameID *entities, nameID toFind, int size) {
    int inf = 0, sup = size - 1;
    while (inf <= sup) {
        int med = ((inf + sup) / 2);
        if (strcmp(entities[med], toFind) == 0) {
            return true; // nel caso passare la posizione
        }
        if (strcmp(toFind, entities[med]) > 0) {
            inf = med + 1;
        } else {
            sup = med - 1;
        }
    }
    return false;
}

int findSender(sender *senders, nameID toFind, int size) {
    int inf = 0, sup = size - 1;

    while (inf <= sup) {
        int med = ((inf + sup) / 2);
        if (strcmp(**senders[med], toFind) == 0) {
            return med; // nel caso passare la posizione
        }
        if (strcmp(toFind, **senders[med]) > 0) {
            inf = med + 1;
        } else {
            sup = med - 1;
        }
    }
    return -1;
}


int findReceiver(struct receiver *receivers, nameID toFind, int size) {
    //scorrere sender e tornare la pos se esiste altrimenti tornare -1
    int inf = 0, sup = size - 1;
    while (inf <= sup) {
        int med = ((inf + sup) / 2);
        if (strcmp(**receivers[med].id, toFind) == 0) {
            return med; // nel caso passare la posizione
        }
        if (strcmp(toFind, **receivers[med].id) > 0) {
            inf = med + 1;
        } else {
            sup = med - 1;
        }
    }
    return -1;
}

int findEntity(struct entityData *entities, nameID toFind, int size) {
    int inf = 0, sup = size - 1;

    while (inf <= sup) {
        int med = ((inf + sup) / 2);
        if (strcmp(entities[med].name, toFind) == 0) {
            return entities[med].puntPos; // nel caso passare la posizione
        }
        if (strcmp(toFind, entities[med].name) > 0) {
            inf = med + 1;
        } else {
            sup = med - 1;
        }
    }
    return -1;
}

void delRelation(struct relation *relation, nameID receiverToRemove, nameID senderToremove, int receiverPos) {
    int senderPos = findSender(relation->receivers[receiverPos].senders, senderToremove,
                               relation->receivers[receiverPos].sendersSize);
    if (senderPos != -1) {//si rimuovono sender e receiver
        //rimozine sender
        for (int i = senderPos; i < relation->receivers[receiverPos].sendersSize - 1; ++i) {
            relation->receivers[receiverPos].senders[i] = relation->receivers[receiverPos].senders[i + 1];
        }
        relation->receivers[receiverPos].sendersSize--;
        if (relation->receivers[receiverPos].sendersSize == 0) {
            for (int i = receiverPos; i < relation->receiversSize - 1; ++i) {
                relation->receivers[i] = relation->receivers[i + 1];
            }
            //relation->receivers[relation->receiversSize].sendersSize = 0;//posto libero
            relation->receiversSize--;
        }

        //Gestione report
        remakeReport(relation, receiverToRemove);
    }
}

int main() {

    struct entityPunt *entities;
    struct entityData *entityNames;
    struct relation relations[10];//numero dei diversi tipi di relazioni
    int relationSize = 0;
    int entitiesSize = 0; //entità allocate
    for (int i = 0; i < 10; i++) {
        relations[i].receiversSize = 0;
        relations[i].report.max = 1;
        relations[i].report.reciversize = 0;
    }
//usati per lettura input
    nameID instruction;
    nameID entity1;
    nameID entity2;
    nameID relation;
    char line[100];
// per ogni istruzione letta si esegue la chiamata alla relativa funzione
    strcpy(instruction, "init");
    while (strcmp(instruction, "end") != 0) {
        fgets(line, 90, stdin);
        sscanf(line, "%s %s %s %s", &instruction, &entity1, &entity2, &relation);
        if (strcmp(instruction, "addent") == 0) {
            int pos = 0;
            if (entitiesSize == 0) {
                entities = malloc(dim * sizeof(struct entityPunt));
                entityNames = malloc(dim * sizeof(struct entityData));
            }
            int positionEntity = findEntity(entityNames, entity1, entitiesSize);
            if (positionEntity == -1) {
                if (entitiesSize % dim == 0 && entitiesSize != 0) {
                    entities = realloc(entities, (entitiesSize + dim) * sizeof(struct entityPunt));
                    entityNames = realloc(entityNames, (entitiesSize + dim) * sizeof(struct entityData));

                }
                //ricerca posizione in cui aggiungere l'elemento nell'array
                while (pos < entitiesSize && strcmp(entityNames[pos].name, entity1) < 0) {
                    pos++;
                }
                //shift a dx degli elementi
                for (int i = entitiesSize - 1; i >= pos; i--) {
                    strcpy(entityNames[i + 1].name, entityNames[i].name);
                    entityNames[i + 1].puntPos = entityNames[i].puntPos;
                    entities[entityNames[i].puntPos].position++;
                    entities[entityNames[i].puntPos].entityName = &entityNames[entities[entityNames[i].puntPos].position].name;
                }

                strcpy(entityNames[pos].name, entity1);

                entityNames[pos].puntPos = entitiesSize;
                entities[entitiesSize].entityName = &entityNames[pos].name;
                entities[entitiesSize].deleted = false;
                entities[entitiesSize].position = pos;
                entitiesSize++;

            } else {//se l'entità gia esiste
                entities[positionEntity].deleted = false;
            }
        } else if (strcmp(instruction, "delent") == 0) {
            if (entitiesSize > 0) {
                int posEntity = findEntity(entityNames, entity1, entitiesSize);
                if (posEntity != -1) {//se l'entità esiste'
                    if (entities[posEntity].deleted == false) {
                        entities[posEntity].deleted = true; //rimossa dalle entità
                        //Rimozione receiver
                        for (int i = 0; i < relationSize; ++i) {
                            int repeat = relations[i].receiversSize;
                            for (int j = 0; j < repeat; ++j) {
                                int receiverPos = findReceiver(relations[i].receivers, entity1,
                                                               relations[i].receiversSize);
                                if (receiverPos != -1) {
                                    free(relations[i].receivers[receiverPos].senders);//va bene in questa posizione??
                                    relations[i].receivers[receiverPos].sendersSize = 0;
                                    for (int k = receiverPos; k < relations[i].receiversSize - 1; ++k) {
                                        relations[i].receivers[k] = relations[i].receivers[k + 1];

                                    }
                                    relations[i].receiversSize--;
                                }
                            }
                        }
                        //rimozione sender
                        for (int i = 0; i < relationSize; ++i) {
                            int receiverNumber = relations[i].receiversSize;
                            for (int j = 0; j < receiverNumber; ++j) {
                                int senderPos = findSender(relations[i].receivers[j].senders, entity1,
                                                           relations[i].receivers[j].sendersSize);

                                if (senderPos != -1) {//rimozione dai senders
                                    for (int k = senderPos; k < relations[i].receivers[j].sendersSize - 1; k++) {
                                        relations[i].receivers[j].senders[k] = relations[i].receivers[j].senders[k + 1];
                                    }

                                    relations[i].receivers[j].sendersSize--;
                                    remakeReport(&relations[i], **relations[i].receivers[j].id);
                                    /**********************/

                                }

                            }

                        }


                        //gestione Report
                        //trovare il receiver che cambia non solo il sender

                        for (int i = 0; i < relationSize; ++i) {
                            remakeReport(&relations[i], entity1);
                        }

                    }
                } // else L'entity non c'è
            }
        } else if (strcmp(instruction, "addrel") == 0) {
            if (entitiesSize > 1) {

                int relationPos = findRelation(relations, relation, relationSize);
                if (relationPos == relationSize) { //bisogna aggiungere la relazione

                    relationPos = 0;
                    while (relationPos < relationSize && strcmp(relations[relationPos].id, relation) < 0) {
                        relationPos++;
                    }
                    for (int i = relationSize - 1; i >= relationPos; i--) {
                        relations[i + 1] = relations[i];

                    }

                    strcpy(relations[relationPos].id, relation);
                    relations[relationPos].receiversSize = 0;
                    relations[relationPos].report.max = 1;
                    relations[relationPos].report.reciversize = 0;
                    relations[relationPos].receiversSize = 0;
                    relationSize++;
                }

                int entity1Pos = findEntity(entityNames, entity1, entitiesSize);
                int entity2Pos = findEntity(entityNames, entity2, entitiesSize);
                int receiverPos = findReceiver(relations[relationPos].receivers, entity2,
                                               relations[relationPos].receiversSize);
                int senderPos = -1;
                if (receiverPos != -1) {
                    senderPos = findSender(relations[relationPos].receivers[receiverPos].senders, entity1,
                                           relations[relationPos].receivers[receiverPos].sendersSize);
                }
                if (entity1Pos != -1 && entity2Pos != -1) { // si verifica che esistano le entità
                    if (entities[entity2Pos].deleted == false &&
                        entities[entity1Pos].deleted == false) { // si verifica che non siano state rimosse
                        if (receiverPos == -1 || senderPos ==
                                                 -1) {// uguale a meno uno significa che almeno una delle due entità non esiste nella relazione e quindi si deve aggiungere una relazione tra quelle entità
                            if (relations[relationPos].receiversSize == 0) {
                                relations[relationPos].receivers = malloc(sizeof(struct receiver) * dim);
                            }

                            if (relations[relationPos].receiversSize != 0 &&
                                (relations[relationPos].receiversSize % dim) == 0) {
                                relations[relationPos].receivers = realloc(relations[relationPos].receivers,
                                                                           sizeof(struct receiver) * (dim +
                                                                                                      relations[relationPos].receiversSize));

                            }
                            //Aggiungo receiver
                            if (receiverPos == -1) {
                                receiverPos = 0;
                                while (receiverPos < relations[relationPos].receiversSize &&
                                       strcmp(**relations[relationPos].receivers[receiverPos].id, entity2) < 0) {
                                    receiverPos++;

                                }

                                for (int i = relations[relationPos].receiversSize - 1; i >= receiverPos; i--) {
                                    relations[relationPos].receivers[i + 1] = relations[relationPos].receivers[i];
                                }
                                relations[relationPos].receivers[receiverPos].id = &entities[entity2Pos].entityName;
                                relations[relationPos].receiversSize++;
                                relations[relationPos].receivers[receiverPos].sendersSize = 0;
                            }

                            //Aggiunta sender
                            if (relations[relationPos].receivers[receiverPos].sendersSize == 0) {
                                relations[relationPos].receivers[receiverPos].senders = malloc(
                                        sizeof(sender) * dim);
                            }
                            if (relations[relationPos].receivers[receiverPos].sendersSize != 0 &&
                                (relations[relationPos].receivers[receiverPos].sendersSize % dim) == 0) {
                                relations[relationPos].receivers[receiverPos].senders = realloc(
                                        relations[relationPos].receivers[receiverPos].senders, sizeof(sender) *
                                                                                               (dim +
                                                                                                relations[relationPos].receivers[receiverPos].sendersSize));
                            }

                            int senderPosRelation = findSender(
                                    relations[relationPos].receivers[receiverPos].senders, entity1,
                                    relations[relationPos].receivers[receiverPos].sendersSize);
                            if (senderPosRelation == -1) {
                                senderPosRelation = 0;
                                while (senderPosRelation <
                                       relations[relationPos].receivers[receiverPos].sendersSize &&
                                       strcmp(**relations[relationPos].receivers[receiverPos].senders[senderPosRelation],
                                              entity1) < 0) {
                                    senderPosRelation++;
                                }

                                for (int i = relations[relationPos].receivers[receiverPos].sendersSize - 1;
                                     i >= senderPosRelation; i--) {
                                    relations[relationPos].receivers[receiverPos].senders[i +
                                                                                          1] = relations[relationPos].receivers[receiverPos].senders[i];
                                }

                                relations[relationPos].receivers[receiverPos].senders[senderPosRelation] = &entities[entity1Pos].entityName;
                                relations[relationPos].receivers[receiverPos].sendersSize++;
                                updateReport(&relations[relationPos],
                                             &relations[relationPos].receivers[receiverPos]);
                            }
                        }
                    }
                }
            }
        } else if (strcmp(instruction, "delrel") == 0) {
            if (entitiesSize > 1) {

                int relationPos = findRelation(relations, relation, relationSize);
                if (relationPos < relationSize) {
                    int receiverPos = findReceiver(relations[relationPos].receivers, entity2,
                                                   relations[relationPos].receiversSize);
                    if (receiverPos != -1) {
                        delRelation(&relations[relationPos], entity2, entity1, receiverPos);

                    }
                }
            }
            //chimata a funzione
        }
        else if (strcmp(instruction, "report") == 0) {
            int none = 0;
            if (relationSize == 0) {
                fputs("none", stdout);
            } else {
                for (int i = 0; i < relationSize; i++) {
                    if (relations[i].report.reciversize != 0) {
                        fputs(relations[i].id, stdout);
                        for (int j = 0; j < relations[i].report.reciversize; ++j) {
                            fputs(" ", stdout);
                            fputs(**relations[i].report.maxReceiver[j].id, stdout);
                        }
                        printf(" %d", relations[i].report.max);
                        fputs("; ", stdout);
                    } else {
                        none++;
                    }
                }
                if (none == relationSize) {
                    fputs("none", stdout);
                }
            }
            fputs("\n", stdout);
        }
    }

    /***DEBUGGING***/
/*
        nameID a,d,*b,*c;
        strcpy(a,"aaaaaaa");
    strcpy(d,"rrrrrrrrrrrr");
    b=&a;
    c=b;
    b=&d;
    //printf("%s",*c);

        for (int k = 0; k < entitiesSize; ++k) {
            printf("%d) nome:%s\t", k,*entities[k].entityName);
            printf("%d) deleted:%d\t", k, entities[k].deleted);
            printf("%d) nome:%s\t", k, entityNames[k].name);
            printf("%d) posizione puntatore:%d\n\n", k, entityNames[k].puntPos);

        }

        printf("           DEBUG         \n");

        printf("RELAZIONE: %s", relations[0].id);
        printf("receiver size:%d\n", relations[0].receiversSize);
        for (int j = 0; j < relations[0].receiversSize; j++) {
            printf("%d\n", relations[0].receivers[j].sendersSize);

            for (int i = 0; i < relations[0].receivers[j].sendersSize; i++) {
                printf("receiver %s \t sender %s \n", **relations[0].receivers[j].id,
                       **relations[0].receivers[j].senders[i]);
            }

        }

        printf("RELAZIONE: %s", relations[1].id);
        printf("receiver size %d\n", relations[1].receiversSize);
        for (int j = 0; j < relations[1].receiversSize; j++) {
            printf("%d\n", relations[1].receivers[j].sendersSize);

            for (int i = 0; i < relations[1].receivers[j].sendersSize; i++) {
                printf("receiver %s \t sender %s \n", **relations[1].receivers[j].id,
                       **relations[1].receivers[j].senders[i]);
               // findEntity(entityNames,**relations[1].receivers[j].id);
            }

        }


        //Per verificare leak

        for (int k = 0; k < relationSize; ++k) {
            for (int i = 0; i < relations[k].receiversSize; ++i) {
                free(relations[k].receivers[i].senders);
            }
            free(relations[k].receivers);
        }
        free(entities);
        free(entityNames);
        /************/
}
