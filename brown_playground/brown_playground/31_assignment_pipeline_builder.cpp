//
//  31_assignment_pipeline_builder.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/8/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
    
    Email() = default;
    
    Email(string f, string t, string b) : from { f }, to { t }, body { b } { }
    
    Email(const Email& other) {
        from = other.from;
        to = other.to;
        body = other.body;
    }
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }
    
protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        nextWorker->Process(move(email));
    }
    
public:
    void SetNext(unique_ptr<Worker> next) { nextWorker = move(next); }
    
private:
    unique_ptr<Worker> nextWorker;
};


class Reader : public Worker {
public:
    Reader(istream& in) : inStream { in } { }
    
    virtual void Run() override {
        string data;
        while (!inStream.eof()) {
            unique_ptr<Email> email = make_unique<Email>();
            
            getline(inStream, data);
            email->from = data;
            
            getline(inStream, data);
            email->to = data;
            
            getline(inStream, data);
            email->body = data;
            
            PassOn(move(email));
        }
    }
    
    void Process(unique_ptr<Email> email) override { }
    
private:
    istream& inStream;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    
public:
    Filter(Function f) : predicate { f } { }
    
    void Process(unique_ptr<Email> email) override {
        if (predicate(*email.get())) {
            PassOn(move(email));
        }
    }
private:
    Function predicate;
};


class Copier : public Worker {
public:
    Copier(string copyTo) : copyToAddress { copyTo } { }
    
    void Process(unique_ptr<Email> email) override {
        bool differentAddress = email->to != copyToAddress;
        //boolalpha ensures string output for bool values ('true' / 'false')
        cout << boolalpha << "different address " << differentAddress << endl << endl;
        
        unique_ptr<Email> copy;
        if (differentAddress) {
            copy = make_unique<Email>(*email.get());
            copy->to = copyToAddress;
        }
        
        PassOn(move(email));
        
        if (differentAddress) {
            PassOn(move(copy));
        }
    }
private:
    string copyToAddress;
};


class Sender : public Worker {
public:
    Sender(ostream& out) : outStream { out } { }
    
    void Process(unique_ptr<Email> email) override {
        outStream
        << email->from << endl
        << email->to << endl
        << email->body << endl;
    }
private:
    ostream& outStream;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        first = make_unique<Reader>(in);
    }
    
    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        assignNextWorker(make_unique<Filter>(filter));
        return *this;
    }
    
    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        assignNextWorker(make_unique<Copier>(recipient));
        return *this;
    }
    
    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        assignNextWorker(make_unique<Sender>(out));
        return *this;
    }
    
    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(first);
    }
private:
    void assignNextWorker(unique_ptr<Worker> next) {
        if (last) {
            Worker* newLast = next.get();
            last->SetNext(move(next));
            last = newLast;
        } else {
            last = next.get();
            first->SetNext(move(next));
        }
    }
    
    unique_ptr<Worker> first;
    Worker* last;
};


void TestSanity() {
    string input = (
                    "erich@example.com\n"
                    "richard@example.com\n"
                    "Hello there\n"
                    
                    "erich@example.com\n"
                    "ralph@example.com\n"
                    "Are you sure you pressed the right button?\n"
                    
                    "ralph@example.com\n"
                    "erich@example.com\n"
                    "I do not make mistakes of that kind\n"
                    );
    istringstream inStream(input);
    ostringstream outStream;
    
    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();
    
    pipeline->Run();
    
    string expectedOutput = (
                             "erich@example.com\n"
                             "richard@example.com\n"
                             "Hello there\n"
                             
                             "erich@example.com\n"
                             "ralph@example.com\n"
                             "Are you sure you pressed the right button?\n"
                             
                             "erich@example.com\n"
                             "richard@example.com\n"
                             "Are you sure you pressed the right button?\n"
                             );
    
    cout << expectedOutput << endl;
    cout << outStream.str() << endl;
    
    assert(expectedOutput == outStream.str());
}

int main31() {
    TestSanity();
    return 0;
}
