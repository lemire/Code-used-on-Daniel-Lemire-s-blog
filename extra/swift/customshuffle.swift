// http://stackoverflow.com/questions/42080624/no-repetition-of-result-following-a-shuffle-of-an-array
// swiftc -o customshuffle customshuffle.swift 
import Foundation

let decksize = 52


let rankStrings = ["A","2","3","4","5","6","7","8","9","10","J","Q","K"]
let suitStrings = ["♠", "♥","♦","♣"]

struct card : Hashable, Equatable, CustomStringConvertible {
    var rank: Int //1,2...,11,12,13
    var suit: Int // 1,2,3,4

    var hashValue: Int {
      return rank + suit
    }
    static func == (lhs: card, rhs: card) -> Bool {
        return lhs.rank == rhs.rank && lhs.suit == rhs.suit
    }

    var description: String {
      return rankStrings[self.rank - 1] + suitStrings[self.suit - 1]
    }
}

// seems like Swift still lacks a portable random number generator
func portablerand(_ max: Int)->Int {
      #if os(Linux)
            return Int(random() % (max + 1))
       #else
            return Int(arc4random_uniform(UInt32(max)))
      #endif
}

// we populate a data structure where the
// cards are partitioned by rank and then suit (this is not essential)

var deck = [[card]]()
for i in 1...13 {
    var thisset = [card]()
    for j in 1...4 {
        thisset.append(card(rank:i,suit:j))
    }
    deck.append(thisset)
}

// we write answer in "answer"
var answer = [card]()
// we pick a card at random, first card is special
var rnd = portablerand(decksize)
answer.append(deck[rnd / 4].remove(at: rnd % 4))
var proba = 1.0 / Double(decksize)
while answer.count < decksize {
  // no matter what, we do not want to repeat this rank
  let lastrank = answer.last!.rank
  var myindices = [Int](deck.indices)
  myindices.remove(at: lastrank - 1)
  var totalchoice = 0
  var maxbag = -1
  for i in myindices {
      totalchoice = totalchoice + deck[i].count
      if  maxbag == -1 || deck[i].count  > deck[maxbag].count {
        maxbag = i
      }
  }
  if 2 * deck[maxbag].count >= totalchoice {
    // we have to pick from maxbag
    rnd = portablerand(deck[maxbag].count)
    answer.append(deck[maxbag].remove(at: rnd))
  } else {
    // any bag will do
    rnd = portablerand(totalchoice)
    proba *= 1.0 / Double(totalchoice)
    for i in myindices {
        if rnd >= deck[i].count {
          rnd = rnd - deck[i].count
        } else {
          answer.append(deck[i].remove(at: rnd))
          break
        }
    }
  }
}


for card in answer {
  print(card)
}
print("probability : ", proba)
