classdef policy_search
    properties
        H
        model
        idx
        dt
        nominal_state
        x
    end

    methods
      
        function obj = policy_search(H, model, idx, dt, nominal_state, x)
            obj.H = H;
            obj.model = model;
            obj.idx = idx;
            obj.dt = dt;
            obj.nominal_state = nominal_state;
            obj.x = x;
        end

        function loss = linear_search(obj, W)
            
            for t=1:obj.H
                % control law:
                dx = compute_dx(obj.nominal_state(:, t), obj.x(:,t));
                delta_u = W * dx;
                % simulate:
                obj.x(:,t+1) = f_heli(obj.x(:,t), delta_u, obj.dt, obj.model, obj.idx);
            end

            loss = norm(obj.x(obj.idx.ned,t) - obj.nominal_state(obj.idx.ned,t));
        end

        % function loss = nn_search(obj, W)

            
        % end

    end

end