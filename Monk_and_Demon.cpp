#include <stdio.h>
#include <stdlib.h>
#include <queue>
using namespace std;

#define goal 0
#define capacity_X 3
#define capacity_Y 3
#define Empty 0

typedef struct
{
    int monk, demon;
    char bank;
} State;

void makeNullState(State *state)
{
    state->monk = Empty;
    state->demon = Empty;
    state->bank = ' ';
}

int outGame(State state)
{
    // =============Cách 1=============
    // if (state.monk < 0 || state.monk > capacity_X || state.demon < 0 || state.demon > capacity_Y)
    //     return 1;
    // if (state.monk != 0)
    //     if (state.monk < state.demon)
    //         return 1;
    //     else if (state.monk > state.demon)
    //     {
    //         int monk = capacity_X - state.monk;
    //         int demon = capacity_Y - state.demon;
    //         if (monk != 0 && monk < demon)
    //             return 1;
    //     }
    // return 0;

    // =============Cách 2=============
    if (state.monk < 0 || state.monk > capacity_X || state.demon < 0 || state.demon > capacity_Y)
        return 1;
    if (    (state.monk == 0 && state.demon <= capacity_Y)
         || (state.monk == capacity_X && state.demon <= capacity_Y)
         ||  state.monk == state.demon
        )
        return 0;
    return 1;
}
int move_1_monk(State cur_state, State *result)
{
    result->demon = cur_state.demon;
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 1;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_2_monk(State cur_state, State *result)
{
    result->demon = cur_state.demon;
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 2;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 2;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_1_demon(State cur_state, State *result)
{
    result->monk = cur_state.monk;
    if (cur_state.bank == 'A')
    {
        result->demon = cur_state.demon - 1;
        result->bank = 'B';
    }
    else
    {
        result->demon = cur_state.demon + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_2_demon(State cur_state, State *result)
{
    result->monk = cur_state.monk;
    if (cur_state.bank == 'A')
    {
        result->demon = cur_state.demon - 2;
        result->bank = 'B';
    }
    else
    {
        result->demon = cur_state.demon + 2;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_1monk_1demon(State cur_state, State *result)
{
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 1;
        result->demon = cur_state.demon - 1;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 1;
        result->demon = cur_state.demon + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int call_operator(State cur_state, State *result, int opt)
{
    switch (opt)
    {

    case 1:
        return move_1_monk(cur_state, result);
    case 2:
        return move_1_demon(cur_state, result);
    case 3:
        return move_2_monk(cur_state, result);
    case 4:
        return move_2_demon(cur_state, result);
    case 5:
        return move_1monk_1demon(cur_state, result);
    default:
        printf("Error call operator\n");
        return 0;
    }
}
int checkGoal(State state)
{
    return (state.monk == 0 && state.demon == 0 && state.bank == 'B');
}
void printState(State state)
{
    printf("    (Monk: %d)-----(Demon: %d)-----(Bank: %c)\n", state.monk, state.demon, state.bank);
}
const char *action[] = {
    "First State",
    "Move 1 Monk",
    "Move 1 Demon",
    "Move 2 Monk",
    "Move 2 Demon",
    "Move 1 Monk and 1 Demon"};
typedef struct Node
{
    State state;
    struct Node *Parent;
    int no_function;
} Node;
int compareState(State state1, State state2)
{
    return state1.monk == state2.monk && state1.demon == state2.demon && state1.bank == state2.bank;
}
int findState(State state, queue<Node *> queue)
{
    while (!queue.empty())
    {
        if (compareState(queue.front()->state, state))
            return 1;
        queue.pop();
    }
    return 0;
}
Node *BFS_Algorithm(State state)
{
    queue<Node *> Open, Close;
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    Open.push(root);
    while (!Open.empty())
    {
        Node *node = Open.front();
        Open.pop();
        Close.push(node);
        if (checkGoal(node->state))
            return node;
        int opt;
        for (opt = 1; opt <= 5; opt++)
        {
            State newState;
            makeNullState(&newState);
            if (call_operator(node->state, &newState, opt))
            {
                if (findState(newState, Open) || findState(newState, Close))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->Parent = node;
                newNode->no_function = opt;
                Open.push(newNode);
            }
        }
    }
    return NULL;
}
void print_WaysToGetGoal(Node *node)
{
    Node a[100];
    int i = 0;
    while (node->Parent != NULL)
    {
        a[i++] = *node;
        node = node->Parent;
    }
    a[i] = *node;
    int no_action = 0;
    while (i >= 0)
    {
        printf("Action %d: %s \n", no_action++, action[a[i].no_function]);
        printState(a[i].state);
        i--;
    }
}
int main()
{
    State state = {3, 3, 'A'};
    Node *node = BFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}