function [current_set_of_features,current_accuracy] =  feature_search_demo(data)
 
current_set_of_features = []; % Initialize an empty set
current_accuracy=[];
 
for i = 1 : size(data,2)-1 
    disp(['On the ',num2str(i),'th level of the search tree'])
    feature_to_add_at_this_level = [];
    best_so_far_accuracy    = 0;    
    
     for k = 1 : size(data,2)-1 
       if isempty(intersect(current_set_of_features,k)) % Only consider adding, if not already added.
        disp(['--Considering adding the ', num2str(k),' feature'])
        accuracy = leave_one_out_cross_validation(data,current_set_of_features+1,k+1);
        
        if accuracy > best_so_far_accuracy 
            best_so_far_accuracy = accuracy;
            feature_to_add_at_this_level = k;            
        end        
      end
     end
    
    current_set_of_features(i) =  feature_to_add_at_this_level;
    current_accuracy(i) = best_so_far_accuracy;
    disp(['On level ', num2str(i),' i added feature ', num2str(feature_to_add_at_this_level), ' to current set'])
        
 end 
end

function accuracy = leave_one_out_cross_validation(wholedata,current_set_of_features,new_feature)
new_current_set_of_feature = [current_set_of_features new_feature];
data = wholedata(:,[1 new_current_set_of_feature]);
number_correctly_classfied = 0;
 
 for i = 1 : size(data,1)
    object_to_classify = data(i,2:size(data,2)); 
    label_object_to_classify = data(i,1);
    
%     nearest_neighbor_distance = inf;
%     nearest_neighbor_location = inf;
%     for k = 1 : size(data,1)
%         if k ~= i
%              distance = sqrt(sum((object_to_classify - data(k,2:size(data,2))).^2));
%              if  distance <   nearest_neighbor_distance
%                     nearest_neighbor_distance = distance;
%                     nearest_neighbor_location = k;
%                     nearest_neighbor_label    = data(nearest_neighbor_location,1);
%              end
%         end        
%     end 
    
    if i == 1
        data_except_i = data(2:size(data,1),:);
        [~,nearest_neighbor_location] = min(sqrt(sum((object_to_classify - data_except_i(:, 2:size(data,2))).^2 , 2)));
        nearest_neighbor_label    = data_except_i(nearest_neighbor_location,1);
    else
        data_except_i = [data(1:i-1,:); data((i+1):size(data,1),:)];
        [~,nearest_neighbor_location] = min(sqrt(sum((object_to_classify - data_except_i(:, 2:size(data,2))).^2 , 2)));
        nearest_neighbor_label    = data_except_i(nearest_neighbor_location,1);
    end
        
        
            
    if label_object_to_classify == nearest_neighbor_label; 
         number_correctly_classfied = number_correctly_classfied + 1;
    end
    
 end 
 accuracy = number_correctly_classfied / size(data,1);
end





