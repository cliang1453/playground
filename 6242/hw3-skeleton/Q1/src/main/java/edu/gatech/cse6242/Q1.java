package edu.gatech.cse6242;
import java.io.IOException;
import java.util.StringTokenizer;
import java.lang.Object;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.util.*;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Q1 {
  
  public static class TokenizerMapper
	    extends Mapper<Object, Text, IntWritable, IntWritable>{

	  private IntWritable src = new IntWritable();
	  private IntWritable w = new IntWritable();

	  public void map(Object key, Text value, Context context
	                ) throws IOException, InterruptedException {
	  StringTokenizer itr = new StringTokenizer(value.toString(),  "\n");
	  while (itr.hasMoreTokens()) {
	  	
	  	String next_itr = itr.nextToken();
	  	String next_split[] = next_itr.split("\t");

        src.set(Integer.parseInt(next_split[0]));
        w.set(Integer.parseInt(next_split[2]));
        context.write(src, w);

	  }
	}
  }



  public static class IntSumReducer
       extends Reducer<IntWritable,IntWritable,IntWritable,IntWritable> {
    private IntWritable result = new IntWritable();

    public void reduce(IntWritable key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int max = 0;
      for (IntWritable val : values) {
        if(val.get() > max){
          max = val.get();
        }
      }

      if(max > 0){
      	result.set(max);
      	context.write(key, result);
      }
      
    }
  }


  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "Q1");

    /* TODO: Needs to be implemented */
    job.setJarByClass(Q1.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(IntWritable.class);
    job.setOutputValueClass(IntWritable.class);

    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }

}

// package edu.gatech.cse6242;

// import java.io.IOException;
// import java.util.StringTokenizer;
// import java.lang.Object;

// import org.apache.hadoop.fs.Path;
// import org.apache.hadoop.conf.Configuration;
// import org.apache.hadoop.io.*;
// import org.apache.hadoop.mapreduce.*;
// import org.apache.hadoop.util.*;
// import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
// import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

// public class Q1 {

//   public static class TokenizerMapper
//        extends Mapper<Object, Text, IntWritable, IntWritable>{

//     private IntWritable email = new IntWritable();
//     private IntWritable weight = new IntWritable();

//     public void map(Object key, Text value, Context context
//                     ) throws IOException, InterruptedException {
//       StringTokenizer itr = new StringTokenizer(value.toString(), "\n");
//       while (itr.hasMoreTokens()) {
//         String line = itr.nextToken();
//         String tokens[] = line.split("\t");

//         email.set(Integer.parseInt(tokens[1]));
//         weight.set(Integer.parseInt(tokens[2]));
//         context.write(email, weight);
//       }
//     }
//   }

//   public static class IntSumReducer
//        extends Reducer<IntWritable,IntWritable,IntWritable,IntWritable> {
//     private IntWritable result = new IntWritable();

//     public void reduce(IntWritable key, Iterable<IntWritable> values,
//                        Context context
//                        ) throws IOException, InterruptedException {
//       int max = -1;
//       for (IntWritable val : values) {
//         if(val.get() > max) max = val.get();
//       }
//       result.set(max);
//       context.write(key, result);
//     }
//   }

//   public static void main(String[] args) throws Exception {
//     Configuration conf = new Configuration();
//     Job job = Job.getInstance(conf, "Q1");

//     job.setJarByClass(Q1.class);
//     job.setMapperClass(TokenizerMapper.class);
//     job.setCombinerClass(IntSumReducer.class);
//     job.setReducerClass(IntSumReducer.class);
//     job.setOutputKeyClass(IntWritable.class);
//     job.setOutputValueClass(IntWritable.class);

//     FileInputFormat.addInputPath(job, new Path(args[0]));
//     FileOutputFormat.setOutputPath(job, new Path(args[1]));
//     System.exit(job.waitForCompletion(true) ? 0 : 1);
//   }
// }
