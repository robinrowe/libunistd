#include <stdio.h>
#include <stdlib.h>

// queue::push/pop
#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue

int main ()
{
  std::queue<int> myqueue;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n";

  do {
    std::cin >> myint;
    myqueue.push (myint);
  } while (myint);

  std::cout << "myqueue contains: ";
  while (!myqueue.empty())
  {
    std::cout << ' ' << myqueue.front();
    myqueue.pop();
  }
  std::cout << '\n';

  return 0;
}

std::vector<t> v;

template <typename T>
class IpcQueue
{	T* data;
	size_t size;
	size_t head;
	size_t tail;
public:
	IpcQueue(T* memory,size_t size)
	:	memory(memory)
	,	size(size)
	,	head(0)
	,	tail(0)
	{}
	bool empty() const
	{	return head == tail;
	}
	size_t count() const
	{	if(head<=tail)
		{	return tail-head;
		}
		return size-head+tail;
	}
	bool full() const
	{	return count() == size();
	}
	bool push(T&& x)
	{	if(full())
		{	return false;
		}
		data[tail]=x;
		tail++;
		if(tail>=size)
		{	tail = 0;
		}
		return true;
	}
	T& front()
	{	return data[head];
	}
	const T& front() const
	{	return data[head];
	}
	bool pop()
	{	if(empty())
		{	return false;
		}
		head++;
		if(head>=size)
		{	head = 0;
		}
		return true;
	}
	

void enqueue(node_t **head, int val) {
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) return;

    new_node->val = val;
    new_node->next = *head;

    *head = new_node;
}

int dequeue(node_t **head) {
    node_t *current, *prev = NULL;
    int retval = -1;

    if (*head == NULL) return -1;

    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->val;
    free(current);
    
    if (prev)
        prev->next = NULL;
    else
        *head = NULL;

    return retval;
}

void print_list(node_t *head) {
    node_t *current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

int main() {
    node_t *head = NULL;
    int ret;

    enqueue(&head, 11);
    enqueue(&head, 22);
    enqueue(&head, 33);
    enqueue(&head, 44);

    print_list(head);
    
    while ((ret=dequeue(&head)) > 0) {
        printf("dequeued %d\n", ret);
    }
    printf("done. head=%p\n", head);

    return 0;
}