require "benchmark"

def execute_rcas(size, in_file)
  cmd = "./release/bm_scalability2 #{size} #{in_file} > \
    experiments/bm_scalability2.#{size}.txt"

  puts cmd
  benchmark = Benchmark.measure { system(cmd) }
  puts benchmark.inspect

  return {
    size: size,
    runtime: benchmark,
  }
end


def execute_experiment(input_sizes, in_file)
  runtimes = input_sizes.collect do |input_size|
    execute_rcas(input_size, in_file)
  end

  puts "size;runtime_s;runtime_h"
  runtimes.each do |data, i|
    puts [
      data[:size],
      data[:runtime].real,
      data[:runtime].real / (60*60),
    ].join(";")
  end
end


gb = 1000000000
input_sizes = [
        25*gb,
        50*gb,
        75*gb,
       100*gb,
       125*gb,
       150*gb,
       175*gb,
       200*gb,
]

execute_experiment(input_sizes, ARGV.shift)
