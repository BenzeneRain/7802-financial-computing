#!/usr/bin/perl

$bin="../bin/optionMCSim";
$params="../data/curveSpec1.csv ../data/curveDataInput1.csv ../data/optionDesc.csv";

system("rm -f parsingYCDefTime.dat");
system("rm -f bindingYCDefTime.dat");
system("rm -f calcVolTime.dat");
system("rm -f pricingTime.dat");

system("rm -f payout1.dat");
system("rm -f payout2.dat");

$round = 100;

@parsingYCDefTime=();
@bindingYCDefTime=();
@calcVolTime=();
@pricingTime=();
@payout1=();
@payout2=();
@payoutBM=();

$progress = 0;
$step = $round * 0.1;
print ("0% ... ");

srand();
for ($i = 0; $i < $round; $i ++)
{
    if($i >= ($progress + $step))
    {
        $progress = $i;
        $percent = $progress * 100 / $round;
        print ("$percent% ... ");
    }

    my @retu = `$bin $params`;
    $j=0;
    while($j < @retu)
    {
        if(@retu[$j] =~ /^Parsing Yield Curve Definitions/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /(\d+(\.\d+)?)us/;
            $match=$1;
            push(@parsingYCDefTime, $match);
            $j ++;
            next;
        }

        if(@retu[$j] =~ /^Binding Yield Curve Data/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /(\d+(\.\d+)?)us/;
            $match=$1;
            push(@bindingYCDefTime, $match);
            $j ++;
            next;
        }

        if(@retu[$j] =~ /Calculating volatility/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /(\d+(\.\d+)?)us/;
            $match=$1;
            push(@calcVolTime, $match);
            $j ++;
            next;
        }

        if(@retu[$j] =~ /Pricing the option/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /(\d+(\.\d+)?)us/;
            $match=$1;
            push(@pricingTime, $match);
            $j ++;
            next;
        }

        if(@retu[$j] =~ /Average pay out of Benchmark Option/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /\d+(\.\d+)?$/;
            $match=$&;
            push(@payoutBM, $match);
            $j ++;
            next;
        }
        
        if(@retu[$j] =~ /Average pay out according to Method 1/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /\d+(\.\d+)?$/;
            $match=$&;
            push(@payout1, $match);
            $j ++;
            next;
        }

        if(@retu[$j] =~ /Average pay out according to Method 2/)
        {
            #print ("@retu[$j]\n");
            @retu[$j] =~ /\d+(\.\d+)?$/;
            $match=$&;
            push(@payout2, $match);
            $j ++;
            next;
        }
        $j ++;
    }

    sleep(1 + rand());
    undef @retu;
}

print ("\n");

$sumparsingYCDefTime = 0;
$sumparsingYCDefTime += $_ for @parsingYCDefTime;
$avgparsingYCDefTime = $sumparsingYCDefTime / $round;

$sdevparsingYCDefTime = 0;
$sdevparsingYCDefTime += ($_ - $avgparsingYCDefTime) * ($_ - $avgparsingYCDefTime) for @parsingYCDefTime;
$sdevparsingYCDefTime = sqrt($sdevparsingYCDefTime / ($round - 1));

$sumbindingYCDefTime = 0;
$sumbindingYCDefTime += $_ for @bindingYCDefTime;
$avgbindingYCDefTime = $sumbindingYCDefTime / $round;

$sdevbindingYCDefTime = 0;
$sdevbindingYCDefTime += ($_ - $avgbindingYCDefTime) * ($_ - $avgbindingYCDefTime) for @bindingYCDefTime;
$sdevbindingYCDefTime = sqrt($sdevbindingYCDefTime / ($round - 1));

$sumcalcVolTime = 0;
$sumcalcVolTime += $_ for @calcVolTime;
$avgcalcVolTime = $sumcalcVolTime / $round;

$sdevcalcVolTime = 0;
$sdevcalcVolTime += ($_ - $avgcalcVolTime) * ($_ - $avgcalcVolTime) for @calcVolTime;
$sdevcalcVolTime = sqrt($sdevcalcVolTime / ($round - 1));

$sumpricingTime = 0;
$sumpricingTime += $_ for @pricingTime;
$avgpricingTime = $sumpricingTime / $round;

$sdevpricingTime = 0;
$sdevpricingTime += ($_ - $avgpricingTime) * ($_ - $avgpricingTime) for @pricingTime;
$sdevpricingTime = sqrt($sdevpricingTime / ($round - 1));

$sumpayoutBM = 0;
$sumpayoutBM += $_ for @payoutBM;
$avgpayoutBM = $sumpayoutBM / $round;

$sdevpayoutBM = 0;
$sdevpayoutBM += ($_ - $avgpayoutBM) * ($_ - $avgpayoutBM) for @payoutBM;
$sdevpayoutBM = sqrt($sdevpayoutBM / ($round - 1));

$sumpayout1 = 0;
$sumpayout1 += $_ for @payout1;
$avgpayout1 = $sumpayout1 / $round;

$sdevpayout1 = 0;
$sdevpayout1 += ($_ - $avgpayout1) * ($_ - $avgpayout1) for @payout1;
$sdevpayout1 = sqrt($sdevpayout1 / ($round - 1));

$sumpayout2 = 0;
$sumpayout2 += $_ for @payout2;
$avgpayout2 = $sumpayout2 / $round;

$sdevpayout2 = 0;
$sdevpayout2 += ($_ - $avgpayout2) * ($_ - $avgpayout2) for @payout2;
$sdevpayout2 = sqrt($sdevpayout2 / ($round - 1));

print ("Avg parsing yield curve definition time = $avgparsingYCDefTime \n");
print ("SDev parsing yield curve definition time = $sdevparsingYCDefTime \n");

print ("Avg binding yield curve data = $avgbindingYCDefTime \n");
print ("SDev binding yield curve data = $sdevbindingYCDefTime \n");

print ("avg calculating volatility time = $avgcalcVolTime \n");
print ("sdev calculating volatility time = $sdevcalcVolTime \n");

print ("avg time for pricing the option = $avgpricingTime \n");
print ("sdev time for pricing the option = $sdevpricingTime \n");

print ("avg payout Benchmark Option = $avgpayoutBM \n");
print ("sdev payout Benchmark Option = $sdevpayoutBM \n");

print ("avg payout 1 = $avgpayout1 \n");
print ("sdev payout 1 = $sdevpayout1 \n");

print ("Avg payout 2 = $avgpayout2 \n");
print ("SDev payout 2 = $sdevpayout2 \n");

