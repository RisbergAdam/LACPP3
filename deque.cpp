#include <cstdlib>

class DQueue
{
  struct QNode
  {
    QNode *left, *right;
    int val;
  };

  QNode *leftSentinel, *rightSentinel;


public:


  DQueue()
  {
    // TODO
    leftSentinel = (QNode *) std::malloc(sizeof(QNode));
    rightSentinel = (QNode *) std::malloc(sizeof(QNode));
    leftSentinel->left = nullptr;
    leftSentinel->right = rightSentinel;
    rightSentinel->right = nullptr;
    rightSentinel->left = leftSentinel;
  }

  void PushLeft(int val)
  {
    // TODO
    QNode * newNode = (QNode *)  std::malloc(sizeof(QNode));
    newNode->val = val;
    __transaction_atomic
    {
      newNode->left = leftSentinel;
      newNode->right = leftSentinel->right;
      leftSentinel->right->left = newNode;
      leftSentinel->right = newNode;
    }

  }

  void PushRight(int val)
  {
    // TODO
    QNode * newNode = (QNode *) std::malloc(sizeof(QNode));
    newNode->val = val;
    __transaction_atomic
    {
      newNode->right = rightSentinel;
      newNode->left = rightSentinel->left;
      rightSentinel->left->right = newNode;
      rightSentinel->left = newNode;
    }
  }

  int PopLeft()
  {
    // TODO
    __transaction_atomic
    {
      if (leftSentinel->right->right != nullptr) {
        leftSentinel->right->right->left = leftSentinel;
        QNode * toBeDeleted = leftSentinel->right;
        leftSentinel->right = leftSentinel->right->right;
        int val = toBeDeleted->val;
        std::free(toBeDeleted);
        return val;
      } else {
        return -1;
      }

    }

  }

  int PopRight()
  {
    __transaction_atomic
    {
      if (rightSentinel->left->left != nullptr) {
        rightSentinel->left->left->right = rightSentinel;
        QNode * toBeDeleted = rightSentinel->left;
        rightSentinel->left = rightSentinel->left->left;
        int val = toBeDeleted->val;
        std::free(toBeDeleted);
        return val;
      } else {
        return -1;
      }

    }
  }

  void FreeSentinels()
  {
    std::free(leftSentinel);
    std::free(rightSentinel);
  }

};
