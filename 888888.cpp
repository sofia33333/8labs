#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <random>
#include <chrono>
#include <locale.h>

using namespace std;
using namespace std::chrono;

#define MAX_VERTICES 100

// Структура для собственной реализации очереди
typedef struct QueueNode {
    int data;
    QueueNode* next;
} QueueNode;

class CustomQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    CustomQueue() : front(nullptr), rear(nullptr) {}

    ~CustomQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(int value) {
        QueueNode* newNode = new QueueNode();
        newNode->data = value;
        newNode->next = nullptr;

        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        }

        QueueNode* temp = front;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
    }

    int peek() {
        if (isEmpty()) {
            return -1;
        }
        return front->data;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};

// Класс для работы с матрицей смежности
class GraphMatrix {
private:
    vector<vector<int>> adjacencyMatrix;
    int size;

public:
    GraphMatrix(int n) : size(n) {
        adjacencyMatrix.resize(n, vector<int>(n, 0));
    }

    // Генерация случайного неориентированного графа
    void generateRandomGraph(double density = 0.3) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (dis(gen) < density) {
                    adjacencyMatrix[i][j] = 1;
                    adjacencyMatrix[j][i] = 1;
                }
            }
        }
    }

    // Вывод матрицы смежности
    void printAdjacencyMatrix() {
        printf("Матрица смежности (%dx%d):\n", size, size);
        printf("   ");
        for (int i = 0; i < size; i++) {
            printf("%2d ", i);
        }
        printf("\n");

        for (int i = 0; i < size; i++) {
            printf("%2d ", i);
            for (int j = 0; j < size; j++) {
                printf("%2d ", adjacencyMatrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Обход в ширину (BFS) с использованием std::queue
    void BFS_matrix(int startVertex) {
        vector<bool> visited(size, false);
        queue<int> q;

        printf("Обход в ширину (матрица смежности), начиная с вершины %d: ", startVertex);

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            printf("%d ", current);

            for (int i = 0; i < size; i++) {
                if (adjacencyMatrix[current][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        printf("\n");
    }

    // Обход в ширину с использованием собственной очереди
    void BFS_customQueue(int startVertex) {
        vector<bool> visited(size, false);
        CustomQueue q;

        printf("Обход в ширину (собственная очередь), начиная с вершины %d: ", startVertex);

        visited[startVertex] = true;
        q.enqueue(startVertex);

        while (!q.isEmpty()) {
            int current = q.peek();
            q.dequeue();
            printf("%d ", current);

            for (int i = 0; i < size; i++) {
                if (adjacencyMatrix[current][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    q.enqueue(i);
                }
            }
        }
        printf("\n");
    }

    // Преобразование матрицы в списки смежности (в обратном порядке)
    vector<list<int>> matrixToAdjListReverse() {
        vector<list<int>> adjList(size);

        for (int i = 0; i < size; i++) {
            for (int j = size - 1; j >= 0; j--) {
                if (adjacencyMatrix[i][j] == 1) {
                    adjList[i].push_back(j);
                }
            }
        }

        return adjList;
    }

    int getSize() const { return size; }
};

// Функция для измерения времени выполнения
double measureTime(void (GraphMatrix::* bfsFunction)(int), GraphMatrix& graph, int startVertex) {
    auto start = high_resolution_clock::now();
    (graph.*bfsFunction)(startVertex);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000000.0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int n;
    printf("Введите количество вершин графа: ");
    scanf_s("%d", &n);

    if (n > MAX_VERTICES || n <= 0) {
        printf("Некорректное количество вершин!\n");
        return 1;
    }

    printf("=== ЗАДАНИЕ 1 ===\n");

    // Создание и генерация графа
    GraphMatrix graphMatrix(n);
    graphMatrix.generateRandomGraph();
    graphMatrix.printAdjacencyMatrix();

    int startVertex;
    printf("Введите начальную вершину для обхода (0-%d): ", n - 1);
    scanf_s("%d", &startVertex);

    if (startVertex < 0 || startVertex >= n) {
        printf("Некорректная начальная вершина!\n");
        return 1;
    }

    // Создание списков смежности в обратном порядке
    auto adjList = graphMatrix.matrixToAdjListReverse();

    printf("Списки смежности (в обратном порядке):\n");
    for (int i = 0; i < n; i++) {
        printf("Вершина %d: ", i);
        for (int neighbor : adjList[i]) {
            printf("%d ", neighbor);
        }
        printf("\n");
    }
    printf("\n");

    graphMatrix.BFS_matrix(startVertex);

    // Выполнение BFS для списков смежности (в обратном порядке)
    printf("Обход в ширину (списки смежности), начиная с вершины %d: ", startVertex);
    vector<bool> visited(n, false);
    queue<int> q;
    visited[startVertex] = true;
    q.push(startVertex);
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        printf("%d ", current);
        for (int neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    printf("\n");

    printf("\n=== ЗАДАНИЕ 2 ===\n");

    graphMatrix.BFS_customQueue(startVertex);

    printf("\n=== СРАВНЕНИЕ ВРЕМЕНИ РАБОТЫ ===\n");

    double timeStandard = measureTime(&GraphMatrix::BFS_matrix, graphMatrix, startVertex);
    double timeCustom = measureTime(&GraphMatrix::BFS_customQueue, graphMatrix, startVertex);

    printf("Время выполнения со стандартной очередью: %.6f секунд\n", timeStandard);
    printf("Время выполнения с собственной очередью: %.6f секунд\n", timeCustom);
    printf("Разница: %.6f секунд\n", timeCustom - timeStandard);

    return 0;
}
