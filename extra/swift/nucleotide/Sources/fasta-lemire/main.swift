/* The Computer Language Benchmarks Game
 http://benchmarksgame.alioth.debian.org/


by Daniel Lemire, based on the C version by Jeremy Zerfas

*/
//let array: [UInt8] = Array(string.utf8)

import Foundation
import Dispatch
let stdout = FileHandle.standardOutput

// This controls the width of lines that are output by this program.
let MAXIMUM_LINE_WIDTH = 60
let buffer_size = 64 * 1024  // use a 64kB printout buffer/
extension String {
    func toASCIICode() -> UInt8 { return self.utf8.map{UInt8($0)}[0] }
}
let endl : UInt8 = "\n".toASCIICode()

typealias nucleotide_info = (letter: UInt8, probability: Float)

let  iub_Nucleotides_Information : [nucleotide_info]=[
    ("a".toASCIICode(), 0.27), ("c".toASCIICode(), 0.12), ("g".toASCIICode(), 0.12), ("t".toASCIICode(), 0.27), ("B".toASCIICode(), 0.02),
    ("D".toASCIICode(), 0.02), ("H".toASCIICode(), 0.02), ("K".toASCIICode(), 0.02), ("M".toASCIICode(), 0.02), ("N".toASCIICode(), 0.02),
    ("R".toASCIICode(), 0.02), ("S".toASCIICode(), 0.02), ("V".toASCIICode(), 0.02), ("W".toASCIICode(), 0.02), ("Y".toASCIICode(), 0.02)];

let  homo_Sapien_Nucleotides_Information: [nucleotide_info]=[
    ("a".toASCIICode(), 0.3029549426680), ("c".toASCIICode(), 0.1979883004921),
    ("g".toASCIICode(), 0.1975473066391), ("t".toASCIICode(), 0.3015094502008)];

fileprivate func repeat_And_Wrap_String(_ string_To_Repeat: [UInt8], _ number_Of_Characters_To_Create : Int) {
    let string_To_Repeat_Length = string_To_Repeat.count
    let buffercap = MAXIMUM_LINE_WIDTH + string_To_Repeat_Length + 1
    let extended_String_To_Repeat = UnsafeMutablePointer<UInt8>.allocate(capacity:buffercap)
    defer {
        extended_String_To_Repeat.deallocate(capacity: buffercap)
    }
    var i = 0
    while(i < buffercap) {
        for j in 0..<string_To_Repeat.count {
            extended_String_To_Repeat[i] = string_To_Repeat[j]
            i = i &+ 1
            if( i == buffercap) {
                break
            }
        }
    }
    var offset = 0
    var current_Number_Of_Characters_To_Create = number_Of_Characters_To_Create
    
    var buffer = UnsafeMutablePointer<UInt8>.allocate(capacity:buffer_size)
    var currentbuffer = buffer
    let endbuffer = buffer.advanced(by: buffer_size)
    defer {
        buffer.deallocate(capacity: buffer_size)
    }
    while current_Number_Of_Characters_To_Create > 0 {
        var line_Length = MAXIMUM_LINE_WIDTH
        if (current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH) {
            line_Length = current_Number_Of_Characters_To_Create
        }
        offset = offset &+ line_Length
        if(offset>string_To_Repeat_Length) {
            offset = offset &- string_To_Repeat_Length
        }
        if(currentbuffer.advanced(by: line_Length + 1) >= endbuffer) {
            stdout.write(Data(bytesNoCopy:buffer, count: currentbuffer - buffer, deallocator: .none))
            currentbuffer = buffer
        }
        currentbuffer.assign(from: extended_String_To_Repeat.advanced(by: offset), count: line_Length)
        currentbuffer[line_Length] = endl
        currentbuffer = currentbuffer.advanced(by: line_Length + 1)
        current_Number_Of_Characters_To_Create = current_Number_Of_Characters_To_Create &- line_Length
    }
    stdout.write(Data(bytesNoCopy:buffer, count: currentbuffer - buffer, deallocator: .none))
}

let IM : UInt32 = 139968
let IMF : Float = Float(IM)
let SEED : UInt32 = 42
var seed = SEED
let RIMF : Float = Float(1)/IMF
let IA : UInt32 = 3877
let IC : UInt32 = 29573

fileprivate func get_LCG_Pseudorandom_Number() -> Float {
    seed=(seed &* IA &+ IC)%IM;
    return RIMF*Float(seed);
}


fileprivate func generate_And_Wrap_Pseudorandom_DNA_Sequence(
    _  nucleotides_Information: [nucleotide_info],
    _ number_Of_Characters_To_Create: Int){
    // assume that number_Of_Nucleotides is small
    let number_Of_Nucleotides = nucleotides_Information.count
    var cumulative_Probabilities = [UInt32](repeating:0,count:number_Of_Nucleotides-1)
    var cumulative_Probability: Float=0.0
    for i in (0..<nucleotides_Information.count-1) {
        cumulative_Probability+=nucleotides_Information[i].probability;
        cumulative_Probabilities[i]=UInt32(cumulative_Probability*IMF);
    }
    let buffercap = MAXIMUM_LINE_WIDTH+1
    let line = UnsafeMutablePointer<UInt8>.allocate(capacity:buffercap)
    defer {
        line.deallocate(capacity: buffercap)
    }
    line[MAXIMUM_LINE_WIDTH] = endl
    var current_Number_Of_Characters_To_Create = number_Of_Characters_To_Create
    
    var buffer = UnsafeMutablePointer<UInt8>.allocate(capacity:buffer_size)
    var currentbuffer = buffer.advanced(by: 0)
    let endbuffer = buffer.advanced(by: buffer_size)
    defer {
        buffer.deallocate(capacity: buffer_size)
    }
    while (current_Number_Of_Characters_To_Create > 0) {
        var line_Length = MAXIMUM_LINE_WIDTH;
        if(current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH) {
            line_Length = current_Number_Of_Characters_To_Create;
        }
        if(currentbuffer.advanced(by: line_Length + 1) >= endbuffer) {
            stdout.write(Data(bytesNoCopy:buffer, count: currentbuffer - buffer, deallocator: .none))
            currentbuffer = buffer
        }
        for column in 0..<line_Length {
                let r = UInt32(IMF * get_LCG_Pseudorandom_Number())
                var count = 0
                for x in cumulative_Probabilities {
                    if(x<=r) {
                        count = count &+ 1;
                    }
                }
                currentbuffer[column]=nucleotides_Information[count].letter;
        }
        currentbuffer[line_Length] = endl
        currentbuffer = currentbuffer.advanced(by: line_Length + 1)
        current_Number_Of_Characters_To_Create = current_Number_Of_Characters_To_Create &- line_Length;
   }
    stdout.write(Data(bytesNoCopy:buffer, count: currentbuffer - buffer, deallocator: .none))
}

// Read command line parameters
let n: Int
if CommandLine.arguments.count > 1 {
    n = Int(CommandLine.arguments[1]) ?? 1000
} else {
    n = 1000
}



print(">ONE Homo sapiens alu")
var homo_Sapiens_Alu : [UInt8] =
"GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGGCGGGCGGATCACCTGAGGTCAGGAGTTCGAGACCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAATACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCAGCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGGAGGCGGAGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCCAGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA".utf8.map({UInt8($0)});
repeat_And_Wrap_String(homo_Sapiens_Alu, 2*n);

print(">TWO IUB ambiguity codes");
print(iub_Nucleotides_Information)
generate_And_Wrap_Pseudorandom_DNA_Sequence(iub_Nucleotides_Information, 3*n);

print(">THREE Homo sapiens frequency");
generate_And_Wrap_Pseudorandom_DNA_Sequence(
    homo_Sapien_Nucleotides_Information, 5*n);


