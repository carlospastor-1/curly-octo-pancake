#ifndef WORDLE_HELPER_H
#define WORDLE_HELPER_H

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

class WordleHelper{
 
    std::vector<std::string> allowed;
    std::vector<std::string> answers;

    std::vector<std::string> green;
    std::vector<std::string> yellow;
    std::vector<std::string> gray;


    bool contains(std::string word, char c){
        for (int i = 0; i < word.length(); i++){
            if (word[i] == c){
                return true;
            }
        }

        return false;
    }
    bool satisfyGreen(std::string word, std::string currGreen) {
        for (int i = 0; i < 5;i++) {
            if (currGreen[i]!= '_') {
                if (word[i] != currGreen[i]) {
                    return false;
            }
            }
        }
        return true;
    }
    bool satisfyYellow(std::string word, std::vector<std::string> yellow) {
        std::string amount;
        for (int i = 0;i<1;i++) {
            for (int j = 0; j < yellow[i].size();j++) {
                if (yellow[i][j]!='_') {
                    amount+=yellow[i][j];
                }
            }
        }
        if (amount == "") {
            return true;
        }
        for (int i = 0; i < amount.length();i++) {
            if (!contains(word,amount[i])) {
                return false;
            }
        }
        for (int i = 0; i < 1;i++) {
            for (int j = 0; j < yellow[i].size();j++) {
                if (yellow[i][j] != '_') {
                    if (!contains(word,yellow[i][j])) {
                        return false;
                    }
                    if (contains(word, yellow[i][j])) {
                        if (yellow[i][j] == word[j]) {
                            return false;
                        }
                        if (yellow[i][j+1] == '_') {
                            return true;
                        }
                        if (amount.length() > 2) {
                        if (contains(word, yellow[i][j+1]) && contains(word, yellow[i][j+2])) {
                            return true;
                        }
                        }
                        else {
                           if (contains(word, yellow[i][j+1]) || contains(word, yellow[i][j+2])) {
                            return true;
                        } 
                        }
                    }
                    if (yellow[i][yellow[i].size()-1] != '_') {
                    return true;;
                    }
                }
            }
        }
        return false;
    }
    bool maxLetters(std::vector<std::string> green, std::vector<std::string> yellow, std::vector<std::string> gray) {
        std::string letters;
        for (int i = 0; i < green.size();i++) {
            for (int j =0; j < green[i].size();j++) {
                if (green[i][j] != '_') {
                    letters+=green[i][j];
                }
            }
        }
        for (int i =0;i<yellow.size();i++) {
            for (int j =0; j < yellow[i].size();j++) {
                if (yellow[i][j] != '_') {
                    letters+=yellow[i][j];
                }
            }
        }
        if (letters.length()<=5) {
            return false;
        }
        return true;
    }
    bool noDupe(std::string word) {
        for (int i = 0; i < word.length();i++) {
        for (int j = i+1; j < word.length();j++) {
            if (word[i] == word[j]) {
                return false;
            }
        }
    }
    return true;
    }
    bool maxLetter_Remove(std::string word, std::vector<std::string> green, std::vector<std::string> yellow) {
        std::string letters;
        for (int i = 0; i < green.size();i++) {
            for (int j =0; j < green[i].size();j++) {
                if (green[i][j] != '_') {
                    letters+=green[i][j];
                }
            }
        }
        for (int i =0;i<yellow.size();i++) {
            for (int j =0; j < yellow[i].size();j++) {
                if (yellow[i][j] != '_' && !contains(letters, yellow[i][j])) {
                    letters+=yellow[i][j];
                }
            }
        }
        for (int i = 0 ; i < word.length();i++) {
            if (word.find(letters[i]) == true) {
                return false;
            }
        }
        return true;
    }

public:
    WordleHelper(){//default constructor
        std::string answersFile = "answers.txt";
        std::string allowedFile = "allowed.txt";

        std::fstream newfile;

        newfile.open(answersFile, std::ios::in);
        if (newfile.is_open()){
            std::string tp;
            
            while(getline(newfile, tp)){
                answers.push_back(tp);//copying words into vector string declared in the class
            }
            newfile.close();
        }
        else {
            throw std::logic_error("Cant read file " + answersFile + ".");
        }

        newfile.open(allowedFile, std::ios::in);
        if (newfile.is_open()){
            std::string tp;
            
            while(getline(newfile, tp)){
                allowed.push_back(tp);//copying words into vector string declared in the class
            }
            newfile.close();
        }
        else {
            throw std::logic_error("Cant read file " + allowedFile + ".");
        }
    }

    void addGreen(std::string feedback){
        if (feedback.size() != 5){
            throw std::logic_error("Green feedback must be exactly 5 characters");
        }
        if (std::any_of(feedback.begin(), feedback.end(), [](char c){
            return c != '_' && (c < 'a' || c > 'z');
        })){
            throw std::logic_error("Green feedback must contain only lowercase letters or underscores");
        }

        green.push_back(feedback);
    }

    void addYellow(std::string feedback){
        if (feedback.size() != 5){
            throw std::logic_error("Yellow feedback must be exactly 5 characters");
        }
        if (std::any_of(feedback.begin(), feedback.end(), [](char c){
            return c != '_' && (c < 'a' || c > 'z');
        })){
            throw std::logic_error("Yellow feedback must contain only lowercase letters or underscores");
        }

        yellow.push_back(feedback);
    }

    void addGray(std::string feedback){
        if (std::any_of(feedback.begin(), feedback.end(), [](char c){
            return c < 'a' && c > 'z';
        })){
            throw std::logic_error("Yellow feedback must contain only lowercase letters");
        }

        gray.push_back(feedback);
    }

    std::vector<std::string>possibleSolutions(){
        std::string outword;
        std::vector<std::string> result = answers, test;
        std::string currGreen = green[0];
        for(int i = 0; i < gray.size(); i++){
           std::string currGray = gray[i]; 
           for(std::string& word : answers){ 
               bool satisfy = true; 
               for(char c: currGray){
                   if (contains(word, c)){
                       satisfy = false; 
                       break;
                   }
               }
               if (!satisfy) {
                result.erase(std::remove(result.begin(), result.end(), word), result.end());
               }
           }
        }
        for (int i = 0; i < green.size();i++) {
            std::string currGreen = green[i];
            for (std::string& word : answers) {
                if (!satisfyGreen(word, currGreen)) {
                    result.erase(std::remove(result.begin(), result.end(), word), result.end());
                }
            }
        }
        for (std::string& word: answers) {
            if (!satisfyYellow(word, yellow)) {
                result.erase(std::remove(result.begin(), result.end(), word), result.end());
            }
        }
        if (maxLetters(green, yellow, gray)){
            for (std::string& word: answers) {
            if (maxLetter_Remove(word, green, yellow)) {
                result.erase(std::remove(result.begin(), result.end(), word), result.end());
            }
            }
        }
    }

    ~WordleHelper(){

    }

    friend struct WordleTester;

};

#endif