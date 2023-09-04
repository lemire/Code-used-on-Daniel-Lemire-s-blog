# frozen_string_literal: true

identifier_start = [*(?a..?z), *(?A..?Z), ?_]
identifier_inner = [*(?a..?z), *(?A..?Z), *(?0..?9), ?_]
megabytes = ARGV.fetch(0) { 10 }.to_i

File.open("input.txt", "w") do |file|
  megabytes.times do |index|
    print "\r%03d/%03d" % [index + 1, megabytes]
    chunk = +""

    while chunk.length < 1024 * 1024
      unit = rand(256).chr

      if identifier_start.include?(unit)
        unit << rand(20).times.map { identifier_inner.sample }.join
      end

      chunk << unit
    end

    file.print(chunk)
  end

  puts
end