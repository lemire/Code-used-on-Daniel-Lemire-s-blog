package main


import (
        "log"
        "encoding/json"
        "encoding/gob"
        "encoding/xml"
        "math/rand"
        "strconv"
        "time"
        "bytes"
        "fmt"
)



const (
        Male=iota
        Female
        Unknown

        ALL=100
)

type Person struct {
        Name string
        Id      string
        Age     int
        Gender  int
}
type Student struct {
        Person Person
        Grade int
        Class   string
        Teacher Person
}

func (s  Student) String() string{
        v,err:=json.Marshal(s)
        if err!=nil {
                log.Fatal("Marshal failed",s)
        }
        return string(v)

}

var students []Student
func init(){
        students=make([]Student,ALL)
        for i:=0;i<ALL;i++ {
                students[i].Grade=rand.Intn(5)
                students[i].Class=strconv.Itoa(students[i].Grade)+",classid"+strconv.Itoa(rand.Intn(10))
                students[i].Person.Name="Student "+strconv.Itoa(i)
                students[i].Person.Id="11001234512345551"+strconv.Itoa(rand.Intn(10))
                students[i].Person.Age=10+rand.Intn(10)
                students[i].Person.Gender=rand.Intn(3)
                students[i].Teacher.Id="22001154365151344"+strconv.Itoa(rand.Intn(10))
                students[i].Teacher.Name="Teacher "+strconv.Itoa(i)
                students[i].Teacher.Age=40+rand.Intn(10)
                students[i].Teacher.Gender=rand.Intn(3)
        }

}

func toJsonBytes() []byte {
        answer,err:=json.Marshal(students)
        if err!=nil {
                log.Fatal(err)
        }
        return answer
}

func loadJsonBytes(input []byte)  []Student {
        ss:=make([]Student,ALL)
        err:=json.Unmarshal(input,&ss)
        if err!=nil {
                log.Fatal(err)
        }
        return ss
}



func toGobBytes() []byte {
        stream := &bytes.Buffer{}
        en:=gob.NewEncoder(stream)
        err:=en.Encode(students)
        if err!=nil {
                log.Fatal(err)
        }
        return stream.Bytes()
}

func loadGobBytes(input []byte) []Student{
        dec:=gob.NewDecoder(bytes.NewBuffer(input))
        ss:=make([]Student,ALL)
        err:=dec.Decode(&ss)
        if err!=nil {
                log.Fatal(err)
        }
        return ss
}



func toXmlBytes() []byte  {
        answer,err:=xml.Marshal(students)
        if err!=nil {
                log.Fatal(err)
        }
        return answer
}
func loadXmlBytes(input []byte) []Student{
        ss:=make([]Student,0,ALL)
        for len(ss) < ALL {
          err:=xml.Unmarshal(input, &ss)
        if err!=nil {
                log.Fatal(err)
        }

        }
        return ss
}


func demo() {
          start:=time.Now()
          jsonbytes:=toJsonBytes()
          ss1:=loadJsonBytes(jsonbytes)
          end:=time.Now()

          log.Printf("Serialization by JSON elapsed: %d us",end.Sub(start)/1000)

          log.Println("serialized size in bytes: ", len(jsonbytes))
          if len(ss1) != len(students) {
            fmt.Println("bug")
          }



          start=time.Now()
          gobbytes:=toGobBytes()
          ss2:=loadGobBytes(gobbytes)
          end=time.Now()


          log.Printf("Serialization by GOB elapsed: %d us",end.Sub(start)/1000)

          log.Println("serialized size in bytes: ", len(gobbytes))
          if len(ss2) != len(students) {
            fmt.Println("bug")
          }

          start=time.Now()
          xmlbytes:=toXmlBytes()
          ss3:=loadXmlBytes(xmlbytes)
          end=time.Now()


          log.Printf("Serialization by XML elapsed: %d us",end.Sub(start)/1000)

          log.Println("serialized size in bytes: ", len(xmlbytes))
          if len(ss3) != len(students) {
            fmt.Println("bug ",len(ss3),len(students))
          }
}

func main(){
  fmt.Println("Array size: ", len(students))

  for i:=0 ; i < 3 ; i++ {
    fmt.Println("trial ", i+1)
    demo()
    fmt.Println()

  }

}
