#include <iostream>
struct Node2 {
    int begin;
    int end;
    Node2 * prev;
    Node2 * next;
};
int A, B, local;
bool closed = false;
Node2 * head = NULL;
Node2 * headway = NULL;
Node2 * first; // перекрытая дорога
Node2 * second; // перекрытая дорога
Node2 * third; // перекрытая дорога
bool possible = false; // мoжно ли добраться из А в В (изначально предполагаем, что нельзя)
Node2 * way = NULL; // голова пути, по которому мы прошли в test
Node2 * wayend = NULL; //  с помощью этого указателя "ходим" по списку с путём, пройденным в test
bool find(Node2 * road){ //
    Node2 * a = way;
    if (a != NULL) {
        bool end = false;
        while (end == false) {
            if ((road->begin == a->begin) && (road->end == a->end)) {
                end = true;
                return true;
            } else {
                if (a->next == NULL) {
                    end = true;
                    return false;
                } else {
                    a = a->next;
                }
            }
        }

    }
    else {
        return false;
    }
}
void scan (){   // сканирует дороги
    FILE * input;
    input = fopen("C:\\Users\\199930\\CLionProjects\\trip\\road.txt","r");
    Node2 * prev = new Node2; // первая сканированная дорога
    head = prev;
    prev->next = NULL;
    prev->prev = NULL;
    fscanf(input,"%i %i",&prev->begin,&prev->end); // сканирование дороги
    while(feof(input) == false){
        Node2 * next = new Node2; // создаём новую дорогу (a  b) (a!=b)
        next->next = NULL;
        next->prev = prev;  // связь между элементами списка (a<-b)
        prev->next = next;  // связь между элементами списка (a->b)
        fscanf(input,"%i %i",&next->begin,&next->end); // сканирование дороги
        prev = next;  // a=b
    }
        fclose(input);
}
void test(){ // проверяет можно ли дойти от города А в город В (с помощью road обходим)(в way записываем путь)
    Node2 * road = head; // путь, по которому программа пошла в test
    road = head;
    bool take = false;
    while (take == false){
        if ((road != first) && (road != second) && (road != third) && (find(road) == false) && (road->begin == local || road->end == local)){ // если нашли дорогу, по которой можно ехать
            if (road->begin == local){ // смещаем город, в котором мы находимся(идём по дороге (по road))
                local = road->end;
            }
            else {
                local = road->begin;
            }
            if (way == NULL) { // мы до этого никуда не ходили
                wayend = new Node2;
                way = wayend;
                wayend->next = NULL;
                wayend->prev = NULL;
                wayend->begin = road->begin;
                wayend->end = road->end;
                printf("way == NULL\n");
            }
            else {
                while (wayend->next != NULL){
                    wayend = wayend->next; // идём  в конец списка, чтобы добавить новый элемент
                }
                wayend->next = new Node2;
                wayend->next->prev = wayend;
                wayend = wayend->next;
                wayend->next = NULL;
                wayend->begin = road->begin;
                wayend->end = road->end;
                printf("add new ellement\n");
            }
            if (local != B) {
                test();
                if (possible == true){
                    take = true;
                }
                road = road->next;
            }
            else { // мы пришли в В
                possible = true;
                way = NULL;
                wayend = NULL;
                local = A;
                take = true;
                printf("we in B\n");
            }
        }
        else { // не нашли дорогу, по которой можно ехать
            if (road->next != NULL) { // перебираем следующие дороги в списке
                road = road->next;
            }
            else { // дороги в списке кончились, ехать некуда => возвращаемся
                take = true;
                if (wayend != NULL) {
                    while (wayend->next != NULL) {
                        wayend = wayend->next; // идём  в конец списка, чтобы удалить элемент
                    }
                    if (local != wayend->begin) {
                        local = wayend->begin;
                    }
                    else {
                        local = wayend->end;
                    }
                    wayend = wayend->prev;
                    printf("back wayend = wayend->prev;\n");
                    if (wayend != NULL) {
                        if (wayend->next != NULL) {
						delete (wayend->next);
					    }
                    }
                    if (wayend != NULL) {
                        wayend->next = NULL;
                        printf("end of roads wayend->next = NULL\n ");
                    }

                }
                
                 if (wayend == NULL) way = NULL;
                
            }
        }
    }
}
void sort(){ // перебирает все возможные комбинации закрытых дорог
    bool end_first = false, end_second = false, end_third = false;
    first = head;
    second = first->next;
    third = second->next;
    while (end_first == false) {
        while (end_second == false) {
            while(end_third == false) {
                test();
                if (possible == false) {
                    printf("It's possible");
                    end_first = true;
                    end_second = true;
                    end_third = true;
                    closed = true;
                }
                possible = false;
                //            printf("\ntest\n");
                if (third->next == NULL) { // является ли точка third последней точкой списка?
                    end_third = true;  // да
                }
                else third = third->next; // нет
            }
            end_third = false;
            if (second->next->next == NULL) {  // является ли точка second предпоследней точкой списка?
                end_second = true; // да
            }
            else { // нет
                second = second->next;
                third = second->next;
            }
        }
        end_second = false;
        if (first->next->next->next == NULL) {  // является ли точка first второй с конца точкой списка?
            end_first = true; // да
        }
        else { // нет
            first = first->next;
            second = first->next;
            third = second->next;
        }
    }
}
int main() {
    scan();
    scanf("%i %i",&A,&B);
    local = A;
    sort();
    if (closed == false) {
        printf("It's impossible");
    }
	system("pause");
    return 0;
}