
public protocol Getter {
 func get(_ index : Int) -> Int
}

public final class Trivial1 : Getter {
  public func get(_ index : Int) -> Int {
    return 1
  }
}


public final class Trivial7 : Getter {
  public func get(_ index : Int) -> Int {
    return 7
  }
}

public func sum(_ g : Getter) -> Int{
  var s = 0
  for i in 1...100 {
     s += g.get(i)
  }
  return s
}

public func sum17(_ g1 : Trivial1, _ g7 : Trivial7) -> Int{
  return sum(g1) + sum(g7)
}

