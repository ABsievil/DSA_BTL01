#include "main.h"

extern int MAXSIZE;

class Node{ //set time customer is added to circle, time start = 0 (maxTime)
public:
		int time;
		string name;
		int energy;
		Node* next;
		Node* prev;
		/* constructor for Node time*/
		Node(): time(-1), name(""),energy(0),next(nullptr),prev(nullptr){}
		Node(int ti, string na, int e): time(ti), name(na),energy(e),next(nullptr),prev(nullptr){} 

		/* constructor for queue*/
		Node(string na, int e) :name(na), energy(e), next(nullptr) {}
		~Node(){next = nullptr; prev= nullptr;}

		bool operator == (const Node* s){
			return (this->name == s->name) && (this->energy == s->energy);
		}
};

class Queue: public Restaurant::customer { //waiter of customer
private:
	//queue note using prev in Node class
	int size;
	int timeCurrent;    //using for headTimeOfQueue
	Node* headTimeOfQueue;  //save time of customer is added to Queue
	Node* nodeTimeOfQueueCurrent;

    customer* front;  // point to first Queue
    customer* rear;   // point to last Queue
	
public:
    Queue() {
		front = nullptr; rear = nullptr;
		size =0;
		timeCurrent = 0;
		headTimeOfQueue = nullptr;
		nodeTimeOfQueueCurrent= nullptr;
	}
	
	int getSize(){return this->size;}
	customer* getHead(){return front;}
	Node* getHeadTimeOfQueue(){ return this->headTimeOfQueue;}
	Node* getLastTimeOfQueue(){return this->nodeTimeOfQueueCurrent;}
	bool isEmpty() const {
        return this->size == 0;
    }

	int idexAtMaxEnergyCusOfQueue(){
		int eMax = std::abs(front->energy);
		customer* cusAt_eMax = front;

		int idex = 0;
		if(this->size ==1) return idex;
		customer* run = front->next;
		for(int i=1; i<size; i++, run = run->next){
			if(eMax < std::abs(run->energy)){
				eMax = std::abs(run->energy);
				cusAt_eMax = run;
				idex = i;
			}
			else if(eMax == std::abs(run->energy)){
				int timeAt_eMax = getTimeNodeInQueue(cusAt_eMax->name, cusAt_eMax->energy);
				int timeAt_Run = getTimeNodeInQueue(run->name, run->energy);
				if(timeAt_eMax < timeAt_Run){
					eMax = std::abs(run->energy);
					cusAt_eMax = run;
					idex = i;
				}
			}
		}
		return idex;
	}

	bool nameHaved(string newName){
		customer* run = front;
		for(int i=0;i< size ; i++, run= run->next){
			if(run->name == newName) return true;
		}
		return false;
	}

	void printQueue(){  
		if(size == 0) {
			return ;
		}
		customer* run = front;
		for(int i=0;i< size ; i++, run= run->next){
			run->print();
		} 
	}

	customer* getCusAtIndex(customer* A,int index){
		customer* run = A;
		for(int i=0;i<index;i++, run = run->next){}

		return run;
	}
	bool prior(customer* s1,customer* s2){
		int e1= std::abs(s1->energy);
		int e2= std::abs(s2->energy);
		if(e1 > e2) return true;
		else if(e1 == e2){   // if t1 > t2 return true, else return false
			Node* run = headTimeOfQueue;
			Node* t1= nullptr; Node* t2= nullptr;

			for(int i=0;i< this->size; i++, run = run->next){
				if((s1->name == run->name) && (t1== nullptr)){
					t1 = run;
				}
				else if((s2->name == run->name) && (t2== nullptr)){
					t2= run;
				}

				if(t1 != nullptr && t2 != nullptr) break;
			}

			if(t1->time < t2->time) return true;
			else return false;
		}
		else return false;
	}

	void setTimeCurrent(int time){this->timeCurrent = time;}
	int getTimeCurent(){return this->timeCurrent;}

    void enqueue(customer* newCus) {
        if (isEmpty()) {
            front = rear = newCus;
        } else {
            rear->next = newCus;
			newCus->prev = rear;
            rear = newCus;
        }

		Node* timeOfNewCus = new Node(timeCurrent,newCus->name,newCus->energy);
		if(headTimeOfQueue == nullptr){
			headTimeOfQueue = timeOfNewCus;
			nodeTimeOfQueueCurrent = headTimeOfQueue;
		}
		else {
			nodeTimeOfQueueCurrent->next = timeOfNewCus;
			timeOfNewCus->prev = nodeTimeOfQueueCurrent;
			nodeTimeOfQueueCurrent= nodeTimeOfQueueCurrent->next;
		}

		size++;
    }
    void dequeue() {
        if (!isEmpty()) {
			customer* run = front;
			string na= front->name;
			int e = front->energy;
            if(this->size==1) front= rear= nullptr;
			else{
				front= front->next;
				front->prev = nullptr;
			}
			delete run;
			run = nullptr;

			/*delete nodeTimeOfQueue*/
			Node* runnode = headTimeOfQueue;
			for(int i=0;i<size;i++, runnode= runnode->next){
				if(runnode->name == na && runnode->energy == e){
					deleteNodeTimeOfQueue(runnode);
					break;
				}
			}

			this->size--;
        }
    }
	int getTimeNodeInQueue(string na, int e){
		Node* runnode = headTimeOfQueue;
		for(int i=0;i<size;i++, runnode= runnode->next){
			if(runnode->name == na && runnode->energy == e){
				return runnode->time;
				break;
			}
		}
		return -1;
	}

	void deleteElementQueue(string na, int e){  //where print element cus want delete 
		customer* run= front;
		for(int i=0;i<size;i++,run= run->next){     //find run is cus want delete in queue
			if(run->name == na && run->energy == e) {
				break;
			}
		}

		/*delete element in queue*/
		if( (run->name == front->name) && (run->energy == front->energy) ){
			if(size==1) front = rear = nullptr;
			else {
				front= front->next;
				front->prev = nullptr;
			}
			delete run;
			run = nullptr;
		}
		else if( (run->name == rear->name) && (run->energy == rear->energy) ){
			rear= rear->prev;
			rear->next= nullptr;
			delete run;
			run = nullptr;
		}
		else {
			customer* preRun = run->prev;
			customer* nextRun = run->next;
			preRun->next= nextRun;
			nextRun->prev = preRun;
			delete run; 
			run= nullptr;
		}

		this->size--;
	}

	void deleteNodeTimeOfQueue(Node* run){
		if(run== headTimeOfQueue){
			if( headTimeOfQueue == nodeTimeOfQueueCurrent) headTimeOfQueue = nullptr;
			else {
				headTimeOfQueue= headTimeOfQueue->next;
				headTimeOfQueue->prev = nullptr;
			}
			delete run;
			run= nullptr;
		}
		else if(run== nodeTimeOfQueueCurrent){
			nodeTimeOfQueueCurrent= nodeTimeOfQueueCurrent->prev;
			nodeTimeOfQueueCurrent->next= nullptr;
			delete run;
			run= nullptr;
		}
		else{
			Node* preRun = run->prev;
			Node* nextRun = run->next;
			preRun->next = nextRun;
			nextRun->prev = preRun;
			delete run;
			run= nullptr;
		}
	}

	void deleteElementInQueue(Node* nodeWantDelete){
		deleteElementQueue(nodeWantDelete->name, nodeWantDelete->energy);
		deleteNodeTimeOfQueue(nodeWantDelete);
	}

	bool findCus(string na, int e, const bool isDeleteMagician){
		customer* run= front;
		for(int i=0;i<size;i++,run= run->next){
			if(run->name == na && run->energy == e) {
				if(isDeleteMagician && run->energy >0) run->print();
				else if(!isDeleteMagician && run->energy <0) run->print();
				return true;
			}
		}
		return false;
	}
    ~Queue() {
        while (!isEmpty()) {
            dequeue();  //delete element in queue and node time in queue
        }
    }
};

class Stack: public Restaurant::customer{
public:
	customer* headNegative;   //so am
	customer* headPositive;   //so duong
	customer* nodeCurrentNegative;
	customer* nodeCurrentPositive;
	int sizeNegative, sizePositive;
	Stack(){
		headNegative= nullptr;
		headPositive= nullptr;
		nodeCurrentNegative = nullptr;
		nodeCurrentPositive = nullptr;
		sizeNegative=0;
		sizePositive=0;
	}
	void push(string na, int e){
		customer* cus = new customer(na,e, nullptr,nullptr);
		if(cus->energy <0){
			if(headNegative == nullptr){
				headNegative = cus;
				nodeCurrentNegative = cus;
			}
			else{
				nodeCurrentNegative->next = cus;
				cus->prev = nodeCurrentNegative;
				nodeCurrentNegative = nodeCurrentNegative->next;
			}

			sizeNegative++;
		}
		else{
			if(headPositive == nullptr){
				headPositive = cus;
				nodeCurrentPositive = cus;
			}
			else{
				nodeCurrentPositive->next = cus;
				cus->prev = nodeCurrentPositive;
				nodeCurrentPositive = nodeCurrentPositive->next;
			}

			sizePositive++;

		}
		
	}
	customer* pop(int num){ // num == -1 then pop negative, else pop positive
		customer* out = nullptr;  //not access to out->next and out->prev
		if(num <0){
			if(sizeNegative>1){
				out = nodeCurrentNegative;
				nodeCurrentNegative= nodeCurrentNegative->prev;
				nodeCurrentNegative->next= nullptr;
			}
			else{
				out = nodeCurrentNegative;
				headNegative = nodeCurrentNegative = nullptr;
			}

			sizeNegative--;
		}
		else {
			if(sizePositive>1){
				out = nodeCurrentPositive;
				nodeCurrentPositive= nodeCurrentPositive->prev;
				nodeCurrentPositive->next= nullptr;
			}
			else{
				out = nodeCurrentPositive;
				headPositive =nodeCurrentPositive = nullptr;
			}
			
			sizePositive--;
		}

		return out;   // sau khi ở hàm nhận out và lấy energy and name, thì phải xóa nó bằng delete con trỏ nhận out
	}
};

class imp_res : public Restaurant
{
	private:
		customer* head= nullptr;
		customer* positionX = nullptr;
		int sizeCus=0; // num customer in Restaurant current
		Queue* queueCustomer;

		Node* headOfTimeCus = nullptr;         //head of list Node, not save value
		Node* NodeOfTimeCusCurrent = nullptr;  //Node current of list Node trong customer's list
		int timeNodeOfTimeCusCurrent= 0;       //index at NodeOfTimeCusCurrent

	public:	
		imp_res() {
			head = new customer();
			head->next = nullptr;

			headOfTimeCus = new Node();
			queueCustomer= new Queue;
		};

		void RED(string name, int energy)
		{
			//cout<<"====RED "<<name<<" "<<energy<<endl;
			customer *cus = new customer (name, energy, nullptr, nullptr);
			bool cusIsHavedInRestaurant = isNameHaved(cus->name);

			if(energy==0 || cusIsHavedInRestaurant == true) {
				delete cus;
				return ;
			}
			else{
				if(head->next == nullptr){ //num Customer current = 0;
					addToPositionX(cus,true);
				}
				else{
					if(sizeCus < MAXSIZE/2){
						if(cus->energy >= positionX->energy){  //add cus to right X
						addToPositionX(cus,true);
						}
						else{ //add cus to left X
						addToPositionX(cus,false);
						}
					}
					else if(sizeCus>= MAXSIZE/2 && sizeCus < MAXSIZE){
						customer* temp = positionX;
						int RESmax = std::abs(temp->energy - cus->energy);
						customer* nodeAtRESmax = temp;
						
						temp= temp->next; //temp at idex = 1
						for(int i=1; i<sizeCus; i++,temp= temp->next){
							int REScurrent = abs(temp->energy - cus->energy);
							if(RESmax < REScurrent) {
								RESmax = REScurrent;
								nodeAtRESmax = temp;
							}
						}

						RESmax = cus->energy - nodeAtRESmax->energy;
						/* set positionX */		
						positionX = nodeAtRESmax;  //set X to RESmax

						if(RESmax>0){ // add to right temp
							addToPositionX(cus,true);
						}
						else {  //add to left temp
							addToPositionX(cus,false);
						}
					}
					else if(queueCustomer->getSize()<MAXSIZE){ //customer add to Queue when sizeCus = MAXSIZE (full customer)
							queueCustomer->setTimeCurrent(timeNodeOfTimeCusCurrent++);
							queueCustomer->enqueue(cus);
					}
					else delete cus;
				}
			}

		}
		void BLUE(int num)
		{	
			//cout << "====BLUE "<< num <<" "<< sizeCus << endl; 
			if(num>= sizeCus || num > MAXSIZE) num = sizeCus;
			deleteCustomers(num); 
			loadQueueToCus(num); 
		}
		void PURPLE()
		{
			//cout << "====PURPLE"<<endl;  
			if(queueCustomer->isEmpty()) return ;
			int numElementInQueueWantSort = queueCustomer->idexAtMaxEnergyCusOfQueue() +1;

			int N= shellsort(queueCustomer->getHead(),numElementInQueueWantSort);

			/*call BLUE func with num = N%MAXSIZE*/
			int numCallBlue = N% MAXSIZE;
			BLUE(numCallBlue) ;
		}
		void REVERSAL()
		{
			//cout << "====REVERSAL" << endl; 
			if(sizeCus==0) return ;
			
			Stack *myStack = new Stack();

			string namePositionXInitial = positionX->name;
			int energyPositionXInitial = positionX->energy;
			customer* run = positionX;
			for(int i=0;i< sizeCus; i++, run= run->prev){  //set value for myStack
				myStack->push(run->name, run->energy);
			}
			
			run = positionX;
			for(int i=0;i<sizeCus;i++, run = run->prev){  //reset value for circle
				customer* cusCurrent = nullptr;
				if(run->energy <0) cusCurrent = myStack->pop(-1);
				else cusCurrent = myStack->pop(1);

				run->name = cusCurrent->name;
				run->energy = cusCurrent->energy;

				delete cusCurrent;
			}

			delete myStack;

			positionX = findCus(namePositionXInitial,energyPositionXInitial);  //reset positionX
		}
		void UNLIMITED_VOID()
		{
			//cout << "unlimited_void" << endl;
			int minEnergy = 0;
			customer* headOfLink = nullptr;
			int sizeOfLink = 0;

			for(int i=4;i<= sizeCus;i++){
				customer* run = positionX; 
				if(i != sizeCus){
					for(int j = 0 ; j <sizeCus ; run = run->next, j++){
						customer* temp = run;
						int energyCurrent = 0;
						for(int k=0; k< i;temp = temp->next, k++){
							energyCurrent+= temp->energy;
						}

						if(i==4 && headOfLink == nullptr) {
							minEnergy = energyCurrent;
							headOfLink = run;
							sizeOfLink = i;
						}
						else if( energyCurrent <= minEnergy){
							minEnergy = energyCurrent;
							headOfLink = run;
							sizeOfLink = i;
						}
					}
				}
				else{
					int energyCurrent = 0;
					for(int j=0; j<sizeCus; run = run->next, j++){
						energyCurrent+= run->energy;
					}

					if(i==4 && headOfLink == nullptr) {
						minEnergy = energyCurrent;
						headOfLink = run;
						sizeOfLink = i;
					}
					else if( energyCurrent <= minEnergy){
						minEnergy = energyCurrent;
						headOfLink = run;
						sizeOfLink = i;
					}
				}
			}

			if(headOfLink != nullptr) {
				customer* run = headOfLink;
				headOfLink = nullptr; //head Of Link, have data
				customer* NodeCurrentOfLink = nullptr; 
				for(int i=0; i<sizeOfLink; i++,run= run->next){
					customer* newCus = new customer(run->name,run->energy,nullptr,nullptr);
					if(headOfLink == nullptr) {
						headOfLink = newCus;
						NodeCurrentOfLink = headOfLink;
					}
					else{
						NodeCurrentOfLink->next = newCus;
						newCus->prev = NodeCurrentOfLink;
						NodeCurrentOfLink= NodeCurrentOfLink->next;
					}
				}

				// cout<<"list finded:"<<endl;
				// customer* X = headOfLink;
				// for(int i=0;i<sizeOfLink;i++, X = X->next){
				// 	X->print();
				// }

				// find min list , note list != circle list in imp_res class
				int idexOfMinElement = -1;
				customer* minElement = findMinListCus(headOfLink,sizeOfLink, idexOfMinElement);

				//cout<<"minElement:"<<minElement->name<<" "<<minElement->energy<<" idex:"<<idexOfMinElement<<endl;

				/*print list */
				customer* temp = minElement;
				for(int i= idexOfMinElement;i<sizeOfLink;i++,temp = temp->next){
					temp->print();
				}
				temp = headOfLink;
				for(int i=0;i<idexOfMinElement;i++, temp = temp->next){
					temp->print();
				}
				
				//delete from headOfLink with sizeOfLink element
				temp = headOfLink;
				for(int i=0;i<sizeOfLink && temp != nullptr;i++){
					customer* temp2 = temp->next;
					delete temp;
					temp = temp2;
				}
			}
		}
		void DOMAIN_EXPANSION()
		{
			//cout << "====DOMAIN_EXPANSION" << endl; 
			//PRINT();
			/* delete all chú linh or chú thuật sư */
			if(sizeCus ==0 && queueCustomer->isEmpty()) return ;
			int energyOfAllCus = 0;
			int energyOfMagician = 0;
			customer* run = this->head;
			if(run != nullptr) {
				run= run->next;
				for(int i=0;i< this->sizeCus ;i++, run = run->next){ //check energy of cus in Table
					energyOfAllCus += run->energy;
					if(run->energy > 0) energyOfMagician+= run->energy;
				}
			}

			if(!queueCustomer->isEmpty()) {
				run = queueCustomer->getHead();
				int sizeQ = queueCustomer->getSize();
				for(int i=0;i < sizeQ; i++, run = run->next){       //check energy of cus in Queue
					energyOfAllCus+= run->energy;
					if(run->energy > 0) energyOfMagician+= run->energy;
				}
			}
			
			/*list time node arranged in table and queue*/
			Node* headTime = nullptr;    //list time combined
			Node* timeCurrent = nullptr;  //list time combined
			addToListTime(headTime,timeCurrent);   //add Time in table and queue

			insertSort(headTime,timeCurrent); //arrange list time with head Node is headTime = selection sort

			bool isdeleteMagician = energyOfMagician < std::abs(energyOfAllCus) ;
			printListDomain(timeCurrent,isdeleteMagician);    //print list from node timeCurrent to headTime

			int numOfdeletedCustomer = 0; 
			for(Node* run = headTime; run!= nullptr; run= run->next){  //delete from headTime to timeCurrent
				bool findAtTable = false;
				Node* nodeWantDelete = findTime(run->time, findAtTable);
				
				if(!isdeleteMagician && nodeWantDelete->energy <0) {  //delete Evil spirit
					if(!findAtTable && !queueCustomer->isEmpty()) {  
						queueCustomer->deleteElementInQueue(nodeWantDelete); //delete evil spirit in Queue
					}
					else if(findAtTable && sizeCus != 0) deleteElementInTable(nodeWantDelete,false); //delete evil spirit in Table

					numOfdeletedCustomer++;
				}
				else if(isdeleteMagician && nodeWantDelete->energy >0){  //delete Magician
					if(!findAtTable && !queueCustomer->isEmpty()) {     
						queueCustomer->deleteElementInQueue(nodeWantDelete);  //delete Magician in Queue
					}
					else if(findAtTable && sizeCus != 0) deleteElementInTable(nodeWantDelete,true);  //delete Magician in Table
					
					numOfdeletedCustomer++;
				}
			}

			/*delete list Time*/
			Node* clear = headTime;
			while (clear!= nullptr)
			{
				Node* temp = clear;
				clear= clear->next;
				delete temp;
			}
			
			/* add to new cus from queue to list */
			if(numOfdeletedCustomer != 0) loadQueueToCus(numOfdeletedCustomer);
		}
		void LIGHT(int num)
		{
			 //cout << "====LIGHT " << num << endl; 
			if(num>0){
				customer* run = positionX;
				for(int i=0;i<sizeCus;i++, run = run->next){
					run->print();
				}
			}
			else if(num<0){
				customer* run = positionX;
				for(int i=0;i<sizeCus;i++, run = run->prev){
					run->print();
				}
			}
			else {
				if(!queueCustomer->isEmpty()){
					queueCustomer->printQueue();
				}
				else return ;
			}
			//PRINT();  //delete PRINT() and printTimeInTable, element print time in queue of printQueue
		}

//support to RED func
		bool isNameHaved(string nameNewCus){
			customer* temp = this->head;
			if(temp->next != nullptr) temp = temp->next;
			else return false;
			for(int i=0; i< this->sizeCus ; i++, temp = temp->next){
				if(temp->name == nameNewCus) return true;   // name cus is haved in list
			}
			bool nameOfCusIsHaveInQueue =false;
			nameOfCusIsHaveInQueue = queueCustomer->nameHaved(nameNewCus);
			
			if(nameOfCusIsHaveInQueue) return true;
			return false;
		}
//support to RED func
		void addToPositionX(customer* cus, bool isRight){ //isRight =true add cus to right X, else add ro left Xs
			if(sizeCus== 0){
				head->next = cus;
				cus->next = cus;
				cus->prev = cus;
			}
			else{
				if(isRight){ //add cus to right X
					if(sizeCus==1){
						positionX->next = cus;
						cus->prev = positionX;
						cus->next= positionX;
						positionX->prev = cus;
					}
					else{
						customer* nextPositionX = positionX->next;

						positionX->next = cus;
						cus->prev = positionX;
						cus->next = nextPositionX;
						nextPositionX->prev = cus;
					}
				}
				else{ //add cus to left X
					if(sizeCus ==1){
						positionX->prev = cus;
						cus->next = positionX;
						cus->prev = positionX;
						positionX->next = cus;
					}
					else{
						customer* prevPositionX = positionX->prev;

						prevPositionX->next = cus;
						cus->prev = prevPositionX;
						cus->next = positionX;
						positionX->prev = cus;
					}
				}
			}

			/*saved time new customer added*/
			Node* temp = new Node(timeNodeOfTimeCusCurrent,cus->name,cus->energy);

			if(headOfTimeCus->next == nullptr){
				headOfTimeCus->next = temp;
				temp->prev = headOfTimeCus;
				NodeOfTimeCusCurrent = temp;
			}
			else{
				NodeOfTimeCusCurrent->next = temp;
				temp->prev = NodeOfTimeCusCurrent;

				NodeOfTimeCusCurrent= NodeOfTimeCusCurrent->next;
			}

            /*update*/
			positionX = cus;
			sizeCus++;
			timeNodeOfTimeCusCurrent++;
		}
//support to BLUE func
		void deleteCustomers(int num){
			Node* run = headOfTimeCus;
			if(run != NULL) run = run->next;
			else return ;

			for( int i=0 ; i< num ; i++){
				string nameCusWantDelete = run->name;
				int energyCusWantDelete = run->energy;
				customer* cusWantDelete = findCus(nameCusWantDelete, energyCusWantDelete);
				
				/* change positionX */
				bool changePositionXtoRight = false;
				if(cusWantDelete->energy >0) changePositionXtoRight =true;
				deleteModuleCus(cusWantDelete, changePositionXtoRight); //delete cusWantDelete
				
				Node* temp = run;
				run = run->next;
				headOfTimeCus->next= run;
				if(run != nullptr) run->prev = headOfTimeCus;
				delete temp;
			}
		}
//support to deleteAllEvilSpirits_OR_AllMagician func and deleteCustomers func
		void deleteModuleCus(customer* D, bool isRight){ 
			if(this->sizeCus <=1) positionX = nullptr; //set PostionX = null because head->null, and ensured at sizeCus ==0 then positionX usen't 
			else if(isRight) positionX = D->next;
			else positionX = D->prev;

			customer* firstCus = head->next;
			if(firstCus->name == D->name && firstCus->energy == D->energy){ //check if D == head->next
				customer* prevD = D->prev;
				customer* nextD = D->next;
				
				head->next = nextD;

				D->next = nullptr;
				D->prev = nullptr;
				prevD->next = nextD;
				nextD->prev = prevD;
				
				delete D;
				D = nullptr;
				this->sizeCus--;
				if(sizeCus==0) head->next = nullptr;
			}
			else if(this->sizeCus > 1){
				customer* prevD = D->prev;
				customer* nextD = D->next;
				D->next = nullptr;
				D->prev = nullptr;
				prevD->next = nextD;
				nextD->prev = prevD;
				delete D;
				D = nullptr;
				this->sizeCus--;
			}
			else if(this->sizeCus ==1){
				D->next = nullptr;
				D->prev = nullptr;
				delete D;
				D = nullptr;
				head->next = nullptr;
				this->sizeCus--;
			}
			else return ; //sizeCus==0
		}
//support to deleteAllEvilSpirits_OR_AllMagician func and deleteCustomers func
		customer* findCus(string name, int energy){
			customer* run = head->next;
			for(int i=0; i< sizeCus; i++ ,run = run->next){
				if(name == run->name && energy == run->energy) return run;
			}
			return nullptr;
		}
//support to BLUE and DOMAIN_EXPANSION func
		void loadQueueToCus(int num){	
		for(int i=0;i<num;i++){
			if(!queueCustomer->isEmpty() && sizeCus < MAXSIZE){
				/*load queue and add = RED func*/
				customer* queueLoading = queueCustomer->getHead(); 
				string firstName = queueLoading->name;
				int firstEnergy = queueLoading->energy;
				
				int tempTimeNode = timeNodeOfTimeCusCurrent;
				timeNodeOfTimeCusCurrent = queueCustomer->getTimeNodeInQueue(firstName,firstEnergy);// set to time cus in queue

				queueCustomer->dequeue();   //delete cus in queue and node time of cus in queue
				RED(firstName,firstEnergy);	
				timeNodeOfTimeCusCurrent = tempTimeNode;  //reset time
			}
			else return ;
		}
		}
//support to DOMAIN_EXPANSION func
		void deleteElementInTable(Node* nodeWantDelete,bool isMagician){
			customer* cusWantDelete = findCus(nodeWantDelete->name, nodeWantDelete->energy);

			deleteModuleCus(cusWantDelete,isMagician); //delete Customer and set positionX = cusWantDelete->prev
			deleteElementNode(nodeWantDelete);
		}
		void deleteElementNode(Node* &temp){
			if( (temp->energy == NodeOfTimeCusCurrent->energy) && (temp->name == NodeOfTimeCusCurrent->name) ){ //if node want delete is last node then reset NodeOfTimeCusCurrent
				if(this->sizeCus >=1) {
					NodeOfTimeCusCurrent = NodeOfTimeCusCurrent->prev;  
					NodeOfTimeCusCurrent->next = nullptr;
				}
				else NodeOfTimeCusCurrent = nullptr;
			}
			else if( (temp->energy == (headOfTimeCus->next)->energy) && (temp->name == (headOfTimeCus->next)->name) ){ //if node want delete is firt node(headOfTimeCus->next) then reset headOfTimeCus
				if(this->sizeCus >=1) {
					headOfTimeCus->next = temp->next;
					(temp->next)->prev = headOfTimeCus;
				}
				else headOfTimeCus->next = nullptr;
			}
			else { //if node is normal
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			delete temp;
		}
		void addToListTime(Node*& headTime, Node*& timeCurrent){ //add Time in table and queue
			/* copy frome time in table to headTime*/
			if(headOfTimeCus != nullptr){
			Node* run = headOfTimeCus->next;	
				for(int i=0; i<sizeCus; i++, run = run->next){
					if(headTime == nullptr){
						headTime = new Node(run->time, run->name, run->energy);
						timeCurrent= headTime;
					}
					else {
						timeCurrent->next = new Node(run->time, run->name, run->energy);
						Node* temp = timeCurrent;
						timeCurrent = timeCurrent->next;
						timeCurrent->prev = temp;
					}
				}	
			}

			/* copy from time in queue to headTime */
			Node* headTimeInQueue = nullptr;
			if(!queueCustomer->isEmpty()) headTimeInQueue = queueCustomer->getHeadTimeOfQueue();
			for(Node* run = headTimeInQueue; run!= nullptr; run = run->next){  // can appear bug at run != nullptr
				if(headTime== nullptr){
					headTime = new Node(run->time, run->name, run->energy);
					timeCurrent = headTime;
				}
				else {
					timeCurrent->next = new Node(run->time, run->name, run->energy);
					Node* temp = timeCurrent;
					timeCurrent = timeCurrent->next;
					timeCurrent->prev = temp;
				}
			}
		}
		void insertSort(Node*& headTime, Node*& timeCurrent){
			 Node* pRight = timeCurrent;
			for (Node* run = timeCurrent->prev; run != nullptr; run = pRight->prev) {
				if (run->time <= pRight->time) pRight = pRight->prev;
				else {
					if(run->prev != nullptr) {
						run->prev->next = pRight; 
						pRight->prev = run->prev;
					}
					else{
						pRight->prev = nullptr;
						headTime= pRight;
					}
					for(Node* tmp = pRight; tmp != nullptr; tmp = tmp->next){
						if(run->time > tmp->time && tmp->next == nullptr){
							tmp->next = run;
							run->prev = tmp;
							run->next = nullptr;
							timeCurrent = run;
							break;
						}
						else if(run->time <= tmp->time){
							tmp->prev->next = run;
							run->prev = tmp->prev;
							run->next = tmp;
							tmp->prev = run;
							break;
						}
					}
				}
				if(pRight == nullptr) break;
			}
		}
		Node* findTime(int value, bool& findAtTable){
			Node* run = queueCustomer->getLastTimeOfQueue();
			int sizeQ = queueCustomer->getSize();
			for(int i=0; i<sizeQ; i++, run = run->prev){
				if(run->time == value) {
					findAtTable = false;
					return run;
				}
			}
			run= nullptr;
			if(NodeOfTimeCusCurrent != nullptr) run= NodeOfTimeCusCurrent;
			for(int i=0;i<sizeCus;i++, run= run->prev){
				if(run->time == value) {
					findAtTable = true;
					return run;
				}
			}
			return nullptr;
		}
		void printListDomain(Node* timeCurrent, const bool isDeleteMagician){
			for(Node* run = timeCurrent; run != nullptr ; run = run->prev){
				bool isHaveInQueue = queueCustomer->findCus(run->name, run->energy, isDeleteMagician); //call and print cus
				if(!isHaveInQueue){
					customer* cusLoading = findCus(run->name, run->energy);
					if(isDeleteMagician && cusLoading->energy >0) cusLoading->print();
					else if(!isDeleteMagician && cusLoading->energy <0) cusLoading->print();
				}
			}
		}
//support to UNLIMITED_VOID func
		customer* findMinListCus(customer* headOfLink, int size, int& indexOfMinElement){
			customer* run = headOfLink;

			int minEnergy = 0;
			customer* minElement = nullptr;
			if(run == nullptr) return nullptr;
			for(int i=0; i<size; i++, run = run->next){
				if(i==0){
					minEnergy = run->energy;
					minElement = run;
					indexOfMinElement= i;
				}
				else if(minEnergy> run->energy){
					minEnergy= run->energy;
					minElement = run;
					indexOfMinElement =i;
				}
			}

			return minElement;
		}
//support to PURPLE func, activity at QUEUE, note: note using prev in Node 
		int inssort2(customer* A,int n, int incr){
			int N=0;
			for(int i= incr; i<n; i+= incr) { //
				for(int j=i; (j>= incr) && queueCustomer->prior(queueCustomer->getCusAtIndex(A,j), queueCustomer->getCusAtIndex(A, j-incr)); j-= incr)
				{
					swap(A,j,j-incr);
					N++;
				}
			}

			return N;			
		}
		int shellsort(customer* A,int n){
			int N=0;
			for(int i=n/2; i>2; i/=2)
				for(int j=0; j<i; j++) N+= inssort2(queueCustomer->getCusAtIndex(A,j),n-j,i);

			N+= inssort2(A,n,1);
			return N;
		}
		void swap(customer* A, int x,int y){
			customer* s1= queueCustomer->getCusAtIndex(A,x);
			customer* s2= queueCustomer->getCusAtIndex(A,y);

			int e = s1->energy;
			string na = s1->name;

			s1->energy = s2->energy;
			s1->name = s2->name;

			s2->energy = e;
			s2->name = na;
		}

		~imp_res(){
			delete queueCustomer;
			queueCustomer= nullptr;

			customer* run = head;
			Node* run2 = headOfTimeCus;
			for(int i=-1;i<sizeCus;i++){
				customer* temp = run;
				run = run->next;

				Node* temp2 = run2;
				run2 = run2->next;

				delete temp;
				delete temp2;
			}
			head=nullptr; 
			headOfTimeCus = nullptr;
		}

		// void printTimeInTable(){ //check nho xoa
		// 	Node* run = headOfTimeCus->next;
		// 	cout<<"--Time in Table:\n";
		// 	for(int i=0;i<sizeCus;i++,run = run->next){
		// 		cout<<run->time<<" "<<run->name<<" "<<run->energy<<endl;
		// 	}
		// }
		// void PRINT(){//check nho xoa
		// 	cout<<"--Cus:\n";
		// 	customer* run = positionX;
		// 	for(int i=0;i<sizeCus;i++, run = run->next){
		// 		cout<<run->name<<" "<<run->energy<<" ";
		// 	}cout<<endl;
		// 	printTimeInTable();
		// 	cout<<"--queue:\n";
		// 	queueCustomer->printQueue();
		// 	cout<<"=============="<<endl;
		// }
};