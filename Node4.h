//
// Created by philip on 08.07.16.
//

#ifndef REFERENCE_NODE4_H
#define REFERENCE_NODE4_H

#include <array>
#include "Node.h"
#include "Leaf.h"

namespace art
{
    class Node4 : public Node {
    public:
        std::array<uint8_t, 4> keys{};
        std::array<Node *, 4> children{};

        Node4(Leaf *leaf, unsigned depth) {
            Key key = leaf->get_key();
            keys[0] = key.chunks[depth];
            children[0] = leaf;

            count = 1;
        }

        virtual size_t max_size() const override {
            return 4;
        }

        virtual Node *insert(const Key &key, unsigned depth) override {
            return nullptr;
        }

        virtual void insert(const uint8_t &key_byte, Node *node) override {
            unsigned pos = 0;
            for (; pos < count && keys[pos] < key_byte; pos++);
            if (pos != count) {
                std::move(keys.begin() + pos, keys.begin() + count, keys.begin() + pos + 1);
                std::move(children.begin() + pos, children.begin() + count, children.begin() + pos + 1);
            }
            keys[pos] = key_byte;
            children[pos] = node;
            count++;
        }

        virtual Node **find(const uint8_t &key_byte) override {
            unsigned pos = 0;
            for (; pos < count && keys[pos] < key_byte; pos++);

            if (keys[pos] == key_byte)
                return &children[pos];
            return nullptr;
        }

        virtual void traverse(unsigned depth) override {
            for (size_t i = 0; i < count; i++)
                children[i]->traverse(depth + 1);
        }

    };
}


#endif //REFERENCE_NODE4_H
