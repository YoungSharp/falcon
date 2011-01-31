/* =================================================================================
File name:      SIN_TAB.H                     
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing 256 entries look-up table for sin(ang) in actual value 

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20                                                  
------------------------------------------------------------------------------ */

#include "dmctype.h"

const float32 sin_tab[] =
{    // SINVAL   	  Index  Angle   Sin(Ang)  
      0,              //   0     0     0  
      0.024541229,    //   1     1.4063     0.024541  
      0.049067674,    //   2     2.8125     0.049068  
      0.073564564,    //   3     4.2188     0.073565  
      0.09801714,    //   4     5.625     0.098017  
      0.12241068,    //   5     7.0313     0.12241  
      0.14673047,    //   6     8.4375     0.14673  
      0.17096189,    //   7     9.8438     0.17096  
      0.19509032,    //   8     11.25     0.19509  
      0.21910124,    //   9     12.6562     0.2191  
      0.24298018,    //   10     14.0625     0.24298  
      0.26671276,    //   11     15.4687     0.26671  
      0.29028468,    //   12     16.875     0.29028  
      0.31368174,    //   13     18.2813     0.31368  
      0.33688985,    //   14     19.6875     0.33689  
      0.35989504,    //   15     21.0937     0.3599  
      0.38268343,    //   16     22.5     0.38268  
      0.40524131,    //   17     23.9063     0.40524  
      0.42755509,    //   18     25.3125     0.42756  
      0.44961133,    //   19     26.7187     0.44961  
      0.47139674,    //   20     28.125     0.4714  
      0.49289819,    //   21     29.5313     0.4929  
      0.51410274,    //   22     30.9375     0.5141  
      0.53499762,    //   23     32.3438     0.535  
      0.55557023,    //   24     33.75     0.55557  
      0.57580819,    //   25     35.1563     0.57581  
      0.5956993,     //   26     36.5625     0.5957  
      0.61523159,    //   27     37.9688     0.61523  
      0.63439328,    //   28     39.375     0.63439  
      0.65317284,    //   29     40.7813     0.65317  
      0.67155895,    //   30     42.1875     0.67156  
      0.68954054,    //   31     43.5938     0.68954  
      0.70710678,    //   32     45     0.70711  
      0.72424708,    //   33     46.4063     0.72425  
      0.74095113,    //   34     47.8125     0.74095  
      0.75720885,    //   35     49.2188     0.75721  
      0.77301045,    //   36     50.625     0.77301  
      0.78834643,    //   37     52.0313     0.78835  
      0.80320753,    //   38     53.4375     0.80321  
      0.81758481,    //   39     54.8437     0.81758  
      0.83146961,    //   40     56.25     0.83147  
      0.84485357,    //   41     57.6562     0.84485  
      0.85772861,    //   42     59.0625     0.85773  
      0.87008699,    //   43     60.4688     0.87009  
      0.88192126,    //   44     61.875     0.88192  
      0.8932243,     //   45     63.2813     0.89322  
      0.90398929,    //   46     64.6875     0.90399  
      0.91420976,    //   47     66.0938     0.91421  
      0.92387953,    //   48     67.5     0.92388  
      0.9329928,     //   49     68.9062     0.93299  
      0.94154407,    //   50     70.3125     0.94154  
      0.94952818,    //   51     71.7188     0.94953  
      0.95694034,    //   52     73.125     0.95694  
      0.96377607,    //   53     74.5313     0.96378  
      0.97003125,    //   54     75.9375     0.97003  
      0.97570213,    //   55     77.3438     0.9757  
      0.98078528,    //   56     78.75     0.98079  
      0.98527764,    //   57     80.1563     0.98528  
      0.98917651,    //   58     81.5625     0.98918  
      0.99247953,    //   59     82.9688     0.99248  
      0.99518473,    //   60     84.375     0.99518  
      0.99729046,    //   61     85.7812     0.99729  
      0.99879546,    //   62     87.1875     0.9988  
      0.99969882,    //   63     88.5938     0.9997  
      1,             //   64     90     1  
      0.99969882,    //   65     91.4062     0.9997  
      0.99879546,    //   66     92.8125     0.9988  
      0.99729046,    //   67     94.2188     0.99729  
      0.99518473,    //   68     95.625     0.99518  
      0.99247953,    //   69     97.0313     0.99248  
      0.98917651,    //   70     98.4375     0.98918  
      0.98527764,    //   71     99.8438     0.98528  
      0.98078528,    //   72     101.25     0.98079  
      0.97570213,    //   73     102.6563     0.9757  
      0.97003125,    //   74     104.0625     0.97003  
      0.96377607,    //   75     105.4688     0.96378  
      0.95694034,    //   76     106.875     0.95694  
      0.94952818,    //   77     108.2813     0.94953  
      0.94154407,    //   78     109.6875     0.94154  
      0.9329928,     //   79     111.0938     0.93299  
      0.92387953,    //   80     112.5     0.92388  
      0.91420976,    //   81     113.9063     0.91421  
      0.90398929,    //   82     115.3125     0.90399  
      0.8932243,     //   83     116.7188     0.89322  
      0.88192126,    //   84     118.125     0.88192  
      0.87008699,    //   85     119.5313     0.87009  
      0.85772861,    //   86     120.9375     0.85773  
      0.84485357,    //   87     122.3437     0.84485  
      0.83146961,    //   88     123.75     0.83147  
      0.81758481,    //   89     125.1563     0.81758  
      0.80320753,    //   90     126.5625     0.80321  
      0.78834643,    //   91     127.9688     0.78835  
      0.77301045,    //   92     129.375     0.77301  
      0.75720885,    //   93     130.7812     0.75721  
      0.74095113,    //   94     132.1875     0.74095  
      0.72424708,    //   95     133.5938     0.72425  
      0.70710678,    //   96     135     0.70711  
      0.68954054,    //   97     136.4063     0.68954  
      0.67155895,    //   98     137.8125     0.67156  
      0.65317284,    //   99     139.2188     0.65317  
      0.63439328,    //   100     140.625     0.63439  
      0.61523159,    //   101     142.0313     0.61523  
      0.5956993,     //   102     143.4375     0.5957  
      0.57580819,    //   103     144.8438     0.57581  
      0.55557023,    //   104     146.25     0.55557  
      0.53499762,    //   105     147.6563     0.535  
      0.51410274,    //   106     149.0625     0.5141  
      0.49289819,    //   107     150.4688     0.4929  
      0.47139674,    //   108     151.875     0.4714  
      0.44961133,    //   109     153.2812     0.44961  
      0.42755509,    //   110     154.6875     0.42756  
      0.40524131,    //   111     156.0938     0.40524  
      0.38268343,    //   112     157.5     0.38268  
      0.35989504,    //   113     158.9063     0.3599  
      0.33688985,    //   114     160.3125     0.33689  
      0.31368174,    //   115     161.7188     0.31368  
      0.29028468,    //   116     163.125     0.29028  
      0.26671276,    //   117     164.5313     0.26671  
      0.24298018,    //   118     165.9375     0.24298  
      0.21910124,    //   119     167.3438     0.2191  
      0.19509032,    //   120     168.75     0.19509  
      0.17096189,    //   121     170.1563     0.17096  
      0.14673047,    //   122     171.5625     0.14673  
      0.12241068,    //   123     172.9688     0.12241  
      0.09801714,    //   124     174.375     0.098017  
      0.073564564,   //   125     175.7812     0.073565  
      0.049067674,   //   126     177.1875     0.049068  
      0.024541229,   //   127     178.5938     0.024541  
       0,            //   128     180     1.2246e-016  
      -0.024541229,  //   129     181.4063     -0.024541  
      -0.049067674,  //   130     182.8125     -0.049068  
      -0.073564564,   //   131     184.2188     -0.073565  
      -0.09801714,    //   132     185.625     -0.098017  
      -0.12241068,    //   133     187.0313     -0.12241  
      -0.14673047,    //   134     188.4375     -0.14673  
      -0.17096189,    //   135     189.8438     -0.17096  
      -0.19509032,    //   136     191.25     -0.19509  
      -0.21910124,    //   137     192.6563     -0.2191  
      -0.24298018,    //   138     194.0625     -0.24298  
      -0.26671276,    //   139     195.4687     -0.26671  
      -0.29028468,    //   140     196.875     -0.29028  
      -0.31368174,    //   141     198.2813     -0.31368  
      -0.33688985,    //   142     199.6875     -0.33689  
      -0.35989504,    //   143     201.0938     -0.3599  
      -0.38268343,    //   144     202.5     -0.38268  
      -0.40524131,    //   145     203.9063     -0.40524  
      -0.42755509,    //   146     205.3125     -0.42756  
      -0.44961133,    //   147     206.7188     -0.44961  
      -0.47139674,    //   148     208.125     -0.4714  
      -0.49289819,    //   149     209.5313     -0.4929  
      -0.51410274,    //   150     210.9375     -0.5141  
      -0.53499762,    //   151     212.3438     -0.535  
      -0.55557023,    //   152     213.75     -0.55557  
      -0.57580819,    //   153     215.1563     -0.57581  
      -0.5956993,     //   154     216.5625     -0.5957  
      -0.61523159,    //   155     217.9688     -0.61523  
      -0.63439328,    //   156     219.375     -0.63439  
      -0.65317284,    //   157     220.7813     -0.65317  
      -0.67155895,    //   158     222.1875     -0.67156  
      -0.68954054,    //   159     223.5938     -0.68954  
      -0.70710678,    //   160     225     -0.70711  
      -0.72424708,    //   161     226.4062     -0.72425  
      -0.74095113,    //   162     227.8125     -0.74095  
      -0.75720885,    //   163     229.2188     -0.75721  
      -0.77301045,    //   164     230.625     -0.77301  
      -0.78834643,    //   165     232.0312     -0.78835  
      -0.80320753,    //   166     233.4375     -0.80321  
      -0.81758481,    //   167     234.8437     -0.81758  
      -0.83146961,    //   168     236.25     -0.83147  
      -0.84485357,    //   169     237.6563     -0.84485  
      -0.85772861,    //   170     239.0625     -0.85773  
      -0.87008699,    //   171     240.4688     -0.87009  
      -0.88192126,    //   172     241.875     -0.88192  
      -0.8932243,     //   173     243.2812     -0.89322  
      -0.90398929,    //   174     244.6875     -0.90399  
      -0.91420976,    //   175     246.0938     -0.91421  
      -0.92387953,    //   176     247.5     -0.92388  
      -0.9329928,     //   177     248.9063     -0.93299  
      -0.94154407,    //   178     250.3125     -0.94154  
      -0.94952818,    //   179     251.7188     -0.94953  
      -0.95694034,    //   180     253.125     -0.95694  
      -0.96377607,    //   181     254.5313     -0.96378  
      -0.97003125,    //   182     255.9375     -0.97003  
      -0.97570213,    //   183     257.3438     -0.9757  
      -0.98078528,    //   184     258.75     -0.98079  
      -0.98527764,    //   185     260.1563     -0.98528  
      -0.98917651,    //   186     261.5625     -0.98918  
      -0.99247953,    //   187     262.9688     -0.99248  
      -0.99518473,    //   188     264.375     -0.99518  
      -0.99729046,    //   189     265.7813     -0.99729  
      -0.99879546,    //   190     267.1875     -0.9988  
      -0.99969882,    //   191     268.5938     -0.9997  
      -1,             //   192     270     -1  
      -0.99969882,    //   193     271.4063     -0.9997  
      -0.99879546,    //   194     272.8125     -0.9988  
      -0.99729046,    //   195     274.2188     -0.99729  
      -0.99518473,    //   196     275.625     -0.99518  
      -0.99247953,    //   197     277.0313     -0.99248  
      -0.98917651,    //   198     278.4375     -0.98918  
      -0.98527764,    //   199     279.8438     -0.98528  
      -0.98078528,    //   200     281.25     -0.98079  
      -0.97570213,    //   201     282.6563     -0.9757  
      -0.97003125,    //   202     284.0625     -0.97003  
      -0.96377607,    //   203     285.4688     -0.96378  
      -0.95694034,    //   204     286.875     -0.95694  
      -0.94952818,    //   205     288.2813     -0.94953  
      -0.94154407,    //   206     289.6875     -0.94154  
      -0.9329928,     //   207     291.0938     -0.93299  
      -0.92387953,    //   208     292.5     -0.92388  
      -0.91420976,    //   209     293.9062     -0.91421  
      -0.90398929,    //   210     295.3125     -0.90399  
      -0.8932243,     //   211     296.7188     -0.89322  
      -0.88192126,    //   212     298.125     -0.88192  
      -0.87008699,    //   213     299.5313     -0.87009  
      -0.85772861,    //   214     300.9375     -0.85773  
      -0.84485357,    //   215     302.3438     -0.84485  
      -0.83146961,    //   216     303.75     -0.83147  
      -0.81758481,    //   217     305.1563     -0.81758  
      -0.80320753,    //   218     306.5625     -0.80321  
      -0.78834643,    //   219     307.9688     -0.78835  
      -0.77301045,    //   220     309.375     -0.77301  
      -0.75720885,    //   221     310.7813     -0.75721  
      -0.74095113,    //   222     312.1875     -0.74095  
      -0.72424708,    //   223     313.5938     -0.72425  
      -0.70710678,    //   224     315     -0.70711  
      -0.68954054,    //   225     316.4063     -0.68954  
      -0.67155895,    //   226     317.8125     -0.67156  
      -0.65317284,    //   227     319.2188     -0.65317  
      -0.63439328,    //   228     320.625     -0.63439  
      -0.61523159,    //   229     322.0313     -0.61523  
      -0.5956993,     //   230     323.4375     -0.5957  
      -0.57580819,    //   231     324.8438     -0.57581  
      -0.55557023,    //   232     326.25     -0.55557  
      -0.53499762,    //   233     327.6563     -0.535  
      -0.51410274,    //   234     329.0625     -0.5141  
      -0.49289819,    //   235     330.4688     -0.4929  
      -0.47139674,    //   236     331.875     -0.4714  
      -0.44961133,    //   237     333.2812     -0.44961  
      -0.42755509,    //   238     334.6875     -0.42756  
      -0.40524131,    //   239     336.0938     -0.40524  
      -0.38268343,    //   240     337.5     -0.38268  
      -0.35989504,    //   241     338.9063     -0.3599  
      -0.33688985,    //   242     340.3125     -0.33689  
      -0.31368174,    //   243     341.7188     -0.31368  
      -0.29028468,    //   244     343.125     -0.29028  
      -0.26671276,    //   245     344.5313     -0.26671  
      -0.24298018,    //   246     345.9375     -0.24298  
      -0.21910124,    //   247     347.3437     -0.2191  
      -0.19509032,    //   248     348.75     -0.19509  
      -0.17096189,    //   249     350.1563     -0.17096  
      -0.14673047,    //   250     351.5625     -0.14673  
      -0.12241068,    //   251     352.9688     -0.12241  
      -0.09801714,    //   252     354.375     -0.098017  
      -0.073564564,   //   253     355.7813     -0.073565  
      -0.049067674,   //   254     357.1875     -0.049068  
      -0.024541229,   //   255     358.5938     -0.024541  
      0               //   256     360      -2.4493e-016  
};

